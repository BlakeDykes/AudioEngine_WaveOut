#include "Wave.h"
#include "Buffer.h"
#include "QC_Callback.h"
#include "QC_WaveFill.h"
#include "QC_WaveStatusUpdate.h"
#include "ThreadManager.h"

Wave::Wave(const HWAVEOUT& hOutHandle, QueuePusher*& playbackPusher
	, BeginPlayFlag& beginPlay, std::future<void>& shutdownFlag, int index)
	: ThreadBase("---- WAVE ----"),
	cvFill(ThreadManager::GetManagedCV()),
	cvBeginPlay(beginPlay),
	fShutdown(shutdownFlag),
	poBuffer(new Buffer(BUFFER_SIZE)), 
	poHeader(new WAVEHDR()), 
	OutHandle(hOutHandle),
	pPlaybackPusher(playbackPusher),
	poStatusUpdate(new QC_WaveStatusUpdate(index, WaveTable::WaveStatus::Open)),
	poWaveFill(new QC_WaveFill(this->poBuffer, std::ref(cvFill.cv))),
	poStatusCallback(new QC_Callback(pPlaybackPusher, (QueueCommand*)poStatusUpdate))
{
}

Wave::~Wave()
{
	assert((poHeader->dwFlags & 8) != 8);

	waveOutUnprepareHeader(OutHandle, poHeader, sizeof(WAVEHDR));

	delete poStatusCallback;
	delete poWaveFill;
	delete poStatusUpdate;
	delete poHeader;
	delete poBuffer;
}

QC_WaveFill* Wave::GetFillCommand(std::future<void>&& playFuture)
{
	this->fPlay = std::move(playFuture);
	return poWaveFill;
}

void Wave::WaitForPlaySignal()
{
	std::unique_lock<std::mutex> mtxFilled(cvFill.mtx);
	cvFill.cv.wait(mtxFilled, [this]() { return !poBuffer->IsEmpty() || ShouldShutdown(); });
	RequestPlay();

	std::unique_lock<std::mutex> m(cvBeginPlay.mtx);
	cvBeginPlay.cv.wait(m);
}

void Wave::WaitForStopSignal()
{
	std::unique_lock<std::mutex> mtxFilled(cvFill.mtx);

	while (!ShouldShutdown())
	{
		cvFill.cv.wait(mtxFilled, [this]() { return !poBuffer->IsEmpty() || ShouldShutdown(); });

		RequestPlay();

		WaitForWrite();
	}
}

void Wave::WaitForWrite()
{
	if (!this->poBuffer->IsEmpty())
	{
		while (!ShouldShutdown()
			&& fPlay.wait_for(0s) != std::future_status::ready)
		{
		}

		if (!ShouldShutdown())
		{
			try
			{
				fPlay.get();
			}
			catch (const std::exception& e)
			{
				AZUL_UNUSED_VAR(e);
				return;
			}

			Write();
		}
	}
}

void Wave::Write()
{
	if (!this->poBuffer->IsEmpty())
	{
		poHeader->dwBufferLength = this->poBuffer->GetCurrSize();

		MMRESULT res = waveOutWrite(OutHandle, poHeader, sizeof(WAVEHDR));
		Debug::out("Wave %d - Write\n", this->poStatusUpdate->GetIndex());

		if (res == MMSYSERR_NOERROR)
		{
			this->poStatusUpdate->SetStatus(WaveTable::WaveStatus::WaitingForData);
			this->poBuffer->SetEmpty();
		}
		else
		{
			switch (res)
			{
			case MMSYSERR_INVALHANDLE: Debug::out("WAVEOUTWRITE FAILURE - MMSYSERR: Invalid handle\n");
			case MMSYSERR_NODRIVER: Debug::out("WAVEOUTWRITE FAILURE - MMSYSERR: Invalid handle\n");
			case MMSYSERR_NOMEM: Debug::out("WAVEOUTWRITE FAILURE - MMSYSERR: Invalid handle\n");
			case WAVERR_UNPREPARED: Debug::out("WAVEOUTWRITE FAILURE - WAVERR: Invalid handle\n");

			}
		}
	}
}

void Wave::RequestPlay()
{
	if (!this->poBuffer->IsEmpty())
	{
		poStatusUpdate->SetStatus(WaveTable::WaveStatus::WaitingPlay);
		pPlaybackPusher->Push(poStatusUpdate);
	}
}

void Wave::ThreadMain()
{
	memset(poHeader, 0x0, sizeof(WAVEHDR));

	poHeader->dwUser = (DWORD_PTR)this->poStatusCallback;
	poHeader->lpData = (LPSTR)poBuffer->GetRawBuffer();

	waveOutPrepareHeader(OutHandle, poHeader, sizeof(WAVEHDR));

	pPlaybackPusher->Push(poStatusUpdate);

	// Waits for initial in-order write from playback
	WaitForPlaySignal();

	// Until stop signal:
	// - waits for fill
	// - requests data
	// - waits for write signal
	// - writes
	WaitForStopSignal();
}

bool Wave::ShouldShutdown()
{
	return ThreadBase::ShouldShutdown() && fShouldShutdown.wait_for(0ms) == std::future_status::ready;
}
