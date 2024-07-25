#include <conio.h>
#include "ThreadManager.h"

ThreadManager::ThreadManager()
	: ShouldShutdown(), ThreadCountZero(), poThreadCount(new ThreadCount(ThreadCountZero))
{
}

ThreadManager::~ThreadManager()
{
	if (!ShouldShutdown.IsFlagged())
	{
		ShutdownThreads();
	}

	delete this->poThreadCount;
}


std::future<void>& ThreadManager::GetShutdownFuture()
{
	return std::ref(GetInstance().ShouldShutdown.f);
}

void ThreadManager::RegisterThreadStart()
{
	GetInstance().poThreadCount->Increment();
}

void ThreadManager::RegisterThreadEnd()
{
	GetInstance().poThreadCount->Decrement();
}

void ThreadManager::ShutdownOnKeyPress()
{
	int ch = 0;

	do
	{
		ch = _getch_nolock();
	} while (ch == 0);

	GetInstance().ShutdownThreads();
}

void ThreadManager::ShutdownThreads()
{
	ShouldShutdown.Flag();
	if (this->poThreadCount->GetCount() != 0)
	{
		std::unique_lock<std::mutex> m(ThreadCountZero.mtx);
		ThreadCountZero.cv.wait(m);
	}
}

ThreadManager& ThreadManager::GetInstance()
{
	static ThreadManager inst;
	return inst;
}

