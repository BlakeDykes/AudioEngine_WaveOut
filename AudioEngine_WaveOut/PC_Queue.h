#pragma once

#include "PlayController.h"

class PC_Queue : public PlayController
{
public:
	PC_Queue(WaveTable* waveTable);
	~PC_Queue() = default;

	virtual void HandleWaitingPlay(int index) override;
	virtual void HandleOpen(int index) override;
	virtual void HandleWaveFinished
	() override;

private:
	int PlayQueue[WaveTable::NUM_WAVES];
	int QueueReadyCount;
	int PlayQueueCount;
};