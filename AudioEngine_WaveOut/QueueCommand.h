#pragma once

#include "QueueIdentifiers.h"

class QC_Callback;
class QueuePusher;
class QueueBaseThread;

class QueueCommand
{
public:
	QueueCommand() = delete;
	QueueCommand(CommandName name);
	QueueCommand(CommandName name, bool preventCallback);

	QueueCommand(const QueueCommand&) = default;
	QueueCommand& operator=(const QueueCommand&) = default;
	virtual ~QueueCommand();

	const char* GetNameString() const;
	CommandName GetName() const;

	virtual void Initialize(QueueBaseThread* execQueue);
	void Execute();

	void PreventCallback();
	void DeleteAfterExecute();
	void InitializeCallback(QueuePusher*& execQueue, QueueCommand* callback);

protected:
	virtual void Cleanup();

	virtual bool P_Execute() = 0;

	CommandName Name;
	QC_Callback* pCallback;
	QueueBaseThread* pExecQueue;
	bool bDeleteSelf;
	bool bPreventCallback;
	char _pad[2];
};