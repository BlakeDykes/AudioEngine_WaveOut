#pragma once

#include "QueueCommand.h"

class QueuePusher;

class QC_Callback : public QueueCommand
{
public:
	QC_Callback(QueuePusher* execQueue, QueueCommand* callback);

protected:
	virtual bool P_Execute() override;

private:
	QueuePusher* pExecQueuePusher;
	QueueCommand* pCallback;
};
