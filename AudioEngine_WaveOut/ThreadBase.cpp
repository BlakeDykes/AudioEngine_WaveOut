#include "ThreadBase.h"
#include "ThreadManager.h"
#include "CircularQueue.h"

ThreadBase::ThreadBase(const char* const pName)
	: BannerBase(pName), 
	mThread(), 
	fShouldShutdown(ThreadManager::GetShutdownFuture())
{
}

ThreadBase::~ThreadBase()
{
	if (this->mThread.joinable()) 
	{
		this->mThread.join();
	}
}

void ThreadBase::Launch()
{
	if (this->mThread.joinable() == false) 
	{
		this->mThread = std::thread(std::ref(*this));
	}
}

bool ThreadBase::ShouldShutdown()
{
	return fShouldShutdown.wait_for(0s) == std::future_status::ready;
}

void ThreadBase::operator()()
{
	START_BANNER;

	ThreadManager::RegisterThreadStart();

	ThreadMain();

	Shutdown();
}

void ThreadBase::Shutdown()
{
	ThreadManager::RegisterThreadEnd();
}

QueueThreadBase::QueueThreadBase(const char* const pName)
	: ThreadBase(pName), InQueue()
{
}

QueuePusher* QueueThreadBase::GetQueuePusher()
{
	return InQueue.GetPusher();
}

