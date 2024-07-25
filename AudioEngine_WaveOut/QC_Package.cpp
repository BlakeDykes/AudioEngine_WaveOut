#include "QC_Package.h"
#include "ThreadBase.h"

QC_Package::QC_Package()
	: QueueCommand(CommandName::Package), 
	CommandCount(0), 
	Commands{}
{
}

QC_Package::QC_Package(CommandName name)
	: QueueCommand(name),
	CommandCount(0),
	Commands{}
{
}

int QC_Package::AddCommand(QueueCommand* pCom)
{
	if (CommandCount < MAX_COMMAND_COUNT)
	{
		Commands[CommandCount++] = pCom;
		return CommandCount;
	}
	else
	{
		return -1;
	}
	
}

int QC_Package::GetCommandCount() const { return this->CommandCount; }

bool QC_Package::P_Execute()
{
	for (int i = 0; i < CommandCount; ++i)
	{
		Commands[i]->Initialize(pExecQueue);
		Commands[i]->Execute();
	}

	return true;
}
