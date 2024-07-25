#include "PlayController.h"
#include "WaveTable.h"

PlayController::PlayController(WaveTable* waveTable)
	: pTable(waveTable), PlayingCount(0)
{
}

int PlayController::GetPlayingCount() const { return PlayingCount; }
