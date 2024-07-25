#pragma once

#include "PlayController.h"

class PC_Playing : public PlayController
{
public:
	PC_Playing(WaveTable* waveTable);
	virtual void HandleWaitingPlay(int index) override;
	virtual void HandleOpen(int index) override;
	virtual void HandleWaveFinished() override;
};