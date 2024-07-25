#pragma once

#include "QueueCommand.h"
#include "WaveTable.h"

class QC_WaveStatusUpdate : public QueueCommand
{
public:
	QC_WaveStatusUpdate(int index, WaveTable::WaveStatus);

	void SetStatus(WaveTable::WaveStatus status);

	int GetIndex() const;

protected:
	virtual bool P_Execute() override;

private:
	int Index;
	WaveTable::WaveStatus Status;
	char _pad2[3];

};
