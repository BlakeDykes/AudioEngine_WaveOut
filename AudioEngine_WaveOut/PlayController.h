#pragma once

#include "WaveTable.h"

class PlayController
{
public:
	PlayController() = delete;

	PlayController(WaveTable* waveTable);
	~PlayController() = default;

	virtual void HandleWaitingPlay(int index) = 0;
	virtual void HandleOpen(int index) = 0;
	virtual void HandleWaveFinished() = 0;

	int GetPlayingCount() const;

protected:
	WaveTable* pTable;
	int PlayingCount;
};