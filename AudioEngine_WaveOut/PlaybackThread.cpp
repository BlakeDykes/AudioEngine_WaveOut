#include "PlaybackThread.h"
#include "QC_Callback.h"
#include "ThreadManager.h"

PlaybackThread::PlaybackThread(QueuePusher* coordPusher)
	: QueueThreadBase("--- PLAYBACK ---"),
	bPlayEmpty(false),
	bPlayStarted(false),
	cvBeginPlay(ThreadManager::GetManagedCV()),
	prShutdown(),
	poTable(new WaveTable(this, coordPusher)),
	WData((DWORD_PTR)this)
{
}

PlaybackThread::~PlaybackThread()
{
	delete poTable;
}

void PlaybackThread::StaticWaveOutProc(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	PlaybackThread* pThis = reinterpret_cast<PlaybackThread*>(dwParam1);
	pThis->WaveOutProc(hWaveOut, uMsg, dwInstance, dwParam1, dwParam2);
}

void PlaybackThread::WaveOutProc(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	//AZUL_UNUSED_VAR(dwParam1);
	AZUL_UNUSED_VAR(dwParam2);
	AZUL_UNUSED_VAR(hWaveOut);
	AZUL_UNUSED_VAR(dwInstance);

	PlaybackThread* playCount = (PlaybackThread*)dwInstance;

	switch (uMsg)
	{
	case WOM_DONE:
	{
		playCount->HandleWaveDone();

		WAVEHDR* waveHdr = (WAVEHDR*)dwParam1;
		QC_Callback* pWaveCallback = (QC_Callback*)waveHdr->dwUser;
		pWaveCallback->Execute();

		break;
	}
	case WOM_CLOSE:
		Trace::out("WOM_CLOSE:\n");
		break;

	case WOM_OPEN:
		Trace::out("WOM_OPEN:\n");
		break;

	default:
		assert(false);
	}

}

bool PlaybackThread::RequestDataRequest()
{
	return !ShouldShutdown();
}

bool PlaybackThread::RequestPlay()
{
	return !ShouldShutdown();
}

WaveTable* PlaybackThread::GetWaveTable()
{
	return this->poTable;
}

void PlaybackThread::BeginPlay()
{
	bPlayStarted = true;
	bPlayEmpty = false;

	cvBeginPlay.cv.notify_all();
	//waveOutRestart(WData.HWaveOut);
}

void PlaybackThread::SignalShutdown()
{
	this->bPlayEmpty = true;
}

void PlaybackThread::ThreadMain()
{
	bool initStatus = InitializeWaveOut();

	if (initStatus)
	{
		LaunchWaves();

		QueueCommand* pCurCommand = nullptr;

		while (!ShouldShutdown())
		{
			//poTable->PlayReadyQueue();

			if (this->InQueue.PopFront(pCurCommand))
			{
				pCurCommand->Initialize((QueueBaseThread*)this);

				pCurCommand->Execute();
			}
		}
	}
}

bool PlaybackThread::ShouldShutdown()
{
	return ThreadBase::ShouldShutdown() && (this->bPlayStarted && this->bPlayEmpty);
}

bool PlaybackThread::InitializeWaveOut()
{
	Debug::out("Initializing WAVEOUT\n");

	WAVEFORMATEX wfx;
	wfx.nSamplesPerSec = 22050; // sample rate 
	wfx.wBitsPerSample = 16;    // number of bits per sample of mono data 
	wfx.nChannels = 2;          // number of channels (i.e. mono, stereo...) 
	wfx.wFormatTag = WAVE_FORMAT_PCM;								// format type 
	wfx.nBlockAlign = (WORD)((wfx.wBitsPerSample >> 3) * wfx.nChannels);	// block size of data 
	wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;		// for buffer estimation 
	wfx.cbSize = 0;

	MMRESULT result = 
		waveOutOpen(&WData.HWaveOut, 
					WData.uDeviceID, 
					&wfx, 
					WData.Callback, 
					WData.CallbackData, 
					WData.CallbackMechanism
		);

	if (result != MMSYSERR_NOERROR)
	{
		fprintf(stderr, "unable to open WAVE_MAPPER device\n");

		switch (result)
		{
		case MMSYSERR_ALLOCATED:
			fprintf(stderr, "--- ERROR: resource is already allocated. ---\n");
			break;
		case MMSYSERR_BADDEVICEID:
			fprintf(stderr, "--- ERROR: device identifier is out of range. ---\n");
			break;
		case MMSYSERR_NODRIVER:
			fprintf(stderr, "--- ERROR: No device driver is present. ---\n");
			break;
		case MMSYSERR_NOMEM:
			fprintf(stderr, "--- ERROR: Unable to allocate or lock memory. ---\n");
			break;
		case WAVERR_BADFORMAT:
			fprintf(stderr, "--- ERROR: Attempted to open with an unsupported waveform-audio format. ---\n");
			break;
		case WAVERR_SYNC:
			fprintf(stderr, "--- ERROR: The device is synchronous but waveOutOpen was called without using the WAVE_ALLOWSYNC flag. ---\n");
			break;
		default: break;
		}
		return false;
	}
	else
	{
		float v = 0.5f;
		unsigned short left = unsigned short(0xFFFF * v);
		unsigned short right = unsigned short(0xFFFF * v);

		unsigned long x = (unsigned int)left + (unsigned int)(right << 16);

		waveOutSetVolume(WData.HWaveOut, x);

		//waveOutPause(WData.HWaveOut);
	}

	return true;
}

void PlaybackThread::LaunchWaves()
{
	//Debug::out("Launching Waves\n");

	poTable->InitializeTable(WData.HWaveOut, GetQueuePusher(), std::ref(cvBeginPlay), prShutdown.f);
}

void PlaybackThread::ProcessIn()
{
	QueueCommand* pCurCommand = nullptr;

	while (this->InQueue.PopFront(pCurCommand))
	{
		pCurCommand->Initialize((QueueBaseThread*)this);

		pCurCommand->Execute();
	}
}

void PlaybackThread::Shutdown()
{
	// Pause to avoid triggering new sounds
	// while unblocking waves
	waveOutPause(WData.HWaveOut);

	prShutdown.pr.set_value();

	ThreadBase::Shutdown();
}

void PlaybackThread::HandleWaveDone()
{
	this->poTable->SignalWaveFinished();
}

PlaybackThread::WaveOutData::WaveOutData(DWORD_PTR callbackData)
	: HWaveOut(),
	uDeviceID(WAVE_MAPPER),
	Callback((DWORD_PTR)PlaybackThread::StaticWaveOutProc),
	CallbackData(callbackData),
	CallbackMechanism(CALLBACK_FUNCTION)
{
}
