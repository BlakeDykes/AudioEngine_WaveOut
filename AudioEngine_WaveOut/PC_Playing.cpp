#include "PC_Playing.h"

PC_Playing::PC_Playing(WaveTable* waveTable)
	:PlayController(waveTable)
{
}

void PC_Playing::HandleWaitingPlay(int index)
{
	pTable->SignalPlay(index);
	++PlayingCount;
}

void PC_Playing::HandleOpen(int index)
{
	AZUL_UNUSED_VAR(index);
}

void PC_Playing::HandleWaveFinished()
{
	if (--PlayingCount <= 0)
	{
		pTable->SignalPlayEmpty();
	}
}
