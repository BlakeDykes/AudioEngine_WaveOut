#include "PC_Queue.h"

PC_Queue::PC_Queue(WaveTable* waveTable)
	: PlayController(waveTable), QueueReadyCount(0), PlayQueueCount(0)
{
}

void PC_Queue::HandleWaitingPlay(int index)
{
	AZUL_UNUSED_VAR(index);
	assert(QueueReadyCount < WaveTable::NUM_WAVES);

	if (++QueueReadyCount == WaveTable::NUM_WAVES)
	{
		pTable->SignalPlayQueueFull(this->PlayQueue, this->PlayQueueCount);
	}

}

void PC_Queue::HandleOpen(int index)
{
	this->PlayQueue[PlayQueueCount++] = index;

	if (PlayQueueCount == WaveTable::NUM_WAVES)
	{
		pTable->SignalPlayQueueReady(this->PlayQueue, this->PlayQueueCount);
	}
}

void PC_Queue::HandleWaveFinished()
{
}
