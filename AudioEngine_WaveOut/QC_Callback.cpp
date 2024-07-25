#include "QC_Callback.h"
#include "QueuePusher.h"


QC_Callback::QC_Callback(QueuePusher* execQueue, QueueCommand* callback)
	: QueueCommand(CommandName::Callback), 
	pExecQueuePusher(execQueue), pCallback(callback)
{
}

bool QC_Callback::P_Execute()
{
	pExecQueuePusher->Push(pCallback);

	return true;
}