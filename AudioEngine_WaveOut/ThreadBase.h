#pragma once

#include "ThreadFlags.h"
#include "CircularQueue.h"

class ThreadBase : public BannerBase
{

public:
	ThreadBase() = delete;
	ThreadBase(const char* const pName);

	ThreadBase(const ThreadBase&) = delete;
	ThreadBase& operator = (const ThreadBase&) = delete;
	virtual ~ThreadBase();

	virtual void Launch();
	virtual bool ShouldShutdown();
	
	void operator()();

protected:
	virtual void ThreadMain() = 0;
	virtual void Shutdown();

	std::thread mThread;
	std::future<void>& fShouldShutdown;
};

class QueueThreadBase : public ThreadBase
{
public:
	QueueThreadBase(const char* const pName);

	QueuePusher* GetQueuePusher();
protected:
	CircularQueue InQueue;

};