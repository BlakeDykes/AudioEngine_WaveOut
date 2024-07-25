#include "QC_WaveStatusUpdate.h"
#include "PlaybackThread.h"

QC_WaveStatusUpdate::QC_WaveStatusUpdate(int index, WaveTable::WaveStatus status)
	: QueueCommand(CommandName::WaveStatusUpdate), Index(index), Status(status), _pad2{}
{
}

void QC_WaveStatusUpdate::SetStatus(WaveTable::WaveStatus status)
{
	this->Status = status;
}

int QC_WaveStatusUpdate::GetIndex() const
{
	return this->Index;
}

bool QC_WaveStatusUpdate::P_Execute()
{
	WaveTable* pTable = ((PlaybackThread*)pExecQueue)->GetWaveTable();
	return pTable->UpdateStatus(Index, Status);
}
