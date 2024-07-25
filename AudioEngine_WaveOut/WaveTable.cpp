#include "WaveTable.h"
#include "QC_Package.h"
#include "PlaybackThread.h"
#include "PC_Queue.h"
#include "PC_Playing.h"

WaveTable::WaveTable(PlaybackThread* playback, QueuePusher* coordPusher)
	: mtx(), 
	pPlayback(playback), 
	pCoordPusher(coordPusher), 
	poPlayController(new PC_Queue(this)),
	OpenWaves(0)
{
	StatusUpdateFuncs[0] = &WaveTable::SetOpen;
	StatusUpdateFuncs[1] = &WaveTable::SetWaitingForData;
	StatusUpdateFuncs[2] = &WaveTable::SetWaitingPlay;
	StatusUpdateFuncs[3] = &WaveTable::SetClosed;

	for (int i = 0; i < NUM_WAVES; ++i)
	{
		pWaves[i] = nullptr;
	}
}

WaveTable::~WaveTable()
{
	delete this->poPlayController;

	for (int i = 0; i < NUM_WAVES; ++i)
	{
		delete pWaves[i];
	}

}

void WaveTable::InitializeTable(const HWAVEOUT& hWaveOut, QueuePusher* pPlaybackPusher
	, BeginPlayFlag& beginPlay, std::future<void>& shutdownFlag)
{
	for (int i = 0; i < NUM_WAVES; ++i)
	{
		pWaves[i] = new Wave(hWaveOut, pPlaybackPusher, beginPlay, shutdownFlag, i);
		pWaves[i]->Launch();
		Table[i].Set(i);
	}
}

bool WaveTable::UpdateStatus(int index, WaveStatus status)
{
	WaveTable::StatusUpdates statFunc = this->StatusUpdateFuncs[(byte)status];

	(this->*statFunc)(index);

	return true;
}

void WaveTable::SetOpen(int index)
{
	this->poPlayController->HandleOpen(index);
}

void WaveTable::SetWaitingForData(int index)
{
	assert(index >= 0 && index < NUM_WAVES);

	// Reset play promise for table[index]
	this->Table[index].prPlay = std::promise<void>();
	std::future<void> playFuture = this->Table[index].prPlay.get_future();

	Debug::out("Wave %d - Requesting data\n", index);

	pCoordPusher->Push((QueueCommand*)this->pWaves[Table[index].WaveIndex]->GetFillCommand(std::move(playFuture)));
}

void WaveTable::SetWaitingPlay(int index)
{
	this->poPlayController->HandleWaitingPlay(index);
}

void WaveTable::SetClosed(int index)
{
	assert(index >= 0 && index < NUM_WAVES);
	AZUL_UNUSED_VAR(index);
	// Close Wave
}

Wave* WaveTable::GetWave(int index)
{
	assert(index >= 0 && index < NUM_WAVES);

	return this->pWaves[index];
}

void WaveTable::SignalPlayQueueReady(int* pQueue, int count)
{
	for (int i = 0; i < count; ++i)
	{
		// Reset play promise for table[index]
		this->Table[pQueue[i]].prPlay = std::promise<void>();
		std::future<void> playFuture = this->Table[pQueue[i]].prPlay.get_future();

		pCoordPusher->Push((QueueCommand*)this->pWaves[Table[pQueue[i]].WaveIndex]->GetFillCommand(std::move(playFuture)));
	}
}

void WaveTable::SignalPlayQueueFull(int* queue, int count)
{
	// Initial in-order write
	for (int i = 0; i < count; ++i)
	{
		this->pWaves[queue[i]]->Write();
	}

	delete this->poPlayController;
	this->poPlayController = new PC_Playing(this);

	this->pPlayback->BeginPlay();
}

void WaveTable::SignalPlay(int index)
{
	Table[index].prPlay.set_value();
}

void WaveTable::SignalWaveFinished()
{
	this->poPlayController->HandleWaveFinished();
}

void WaveTable::SignalPlayEmpty()
{
	this->pPlayback->SignalShutdown();
}

WaveTable::WaveEntry::WaveEntry()
	: WaveIndex(-1)
{
}

void WaveTable::WaveEntry::Set(int index)
{
	this->WaveIndex = index;
}


