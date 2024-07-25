#include "QueueCommand.h"
#include "QC_Callback.h"

QueueCommand::QueueCommand(CommandName name)
	: Name(name), 
	pCallback(nullptr),
	pExecQueue(nullptr),
	bDeleteSelf(false), 
	bPreventCallback(false), 
	_pad{}
{
}

QueueCommand::QueueCommand(CommandName name, bool preventCallback)
	: Name(name),
	pCallback(nullptr),
	pExecQueue(nullptr),
	bDeleteSelf(false),
	bPreventCallback(preventCallback),
	_pad{}
{
}

QueueCommand::~QueueCommand()
{
}

void QueueCommand::Initialize(QueueBaseThread* execQueue)
{
	this->pExecQueue = execQueue;
}

void QueueCommand::Execute()
{
	if (P_Execute() && pCallback && !bPreventCallback)
	{
		pCallback->Execute();
		delete pCallback;
	}

	Cleanup();
}

void QueueCommand::Cleanup()
{
	if (bDeleteSelf)
	{
		delete this;
	}
}

const char* QueueCommand::GetNameString() const
{
	return Stringify(this->Name);
}

CommandName QueueCommand::GetName() const
{
	return this->Name;
}

void QueueCommand::PreventCallback()
{
	this->bPreventCallback = true;
}

void QueueCommand::DeleteAfterExecute()
{
	this->bDeleteSelf = true;
}

void QueueCommand::InitializeCallback(QueuePusher*& execQueue, QueueCommand* callback)
{
	this->pCallback = new QC_Callback(execQueue, callback);
}
