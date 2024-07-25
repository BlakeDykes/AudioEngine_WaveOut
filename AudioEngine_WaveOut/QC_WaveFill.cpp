#include "QC_WaveFill.h"
#include "Buffer.h"
#include "CoordinatorThread.h"

QC_WaveFill::QC_WaveFill(Buffer* buffer, std::condition_variable& cv)
	: QueueCommand(CommandName::WaveFill), pBuffer(buffer), CV(cv)
{
}


bool QC_WaveFill::P_Execute()
{
	((CoordinatorThread*)pExecQueue)->PushTo(pBuffer);
	CV.notify_one();
	return true;
}
