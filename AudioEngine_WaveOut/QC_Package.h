#pragma once

#include "QueueCommand.h"

class QueueBaseThread;

class QC_Package : public QueueCommand
{
public:
	static const int MAX_COMMAND_COUNT = 20;

	QC_Package();
	QC_Package(CommandName name);

	int AddCommand(QueueCommand* pCom);
	int GetCommandCount() const;

protected:
	virtual bool P_Execute() override;

	int CommandCount;
	QueueCommand* Commands[MAX_COMMAND_COUNT];
};