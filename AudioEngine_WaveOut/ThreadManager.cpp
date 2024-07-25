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

	for (size_t i = 0; i < CVs.size(); ++i)
	{
		delete CVs[i];
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

CVFlag& ThreadManager::GetManagedCV()
{
	ThreadManager* inst = &GetInstance();

	CVFlag* v = new CVFlag();
	assert(v != nullptr);

	inst->CVs.push_back(v);

	return std::ref(*v);
}

bool ThreadManager::NotifyAll()
{
	for (size_t i = 0; i < CVs.size(); ++i)
	{
		CVs[i]->cv.notify_all();
	}

	return this->poThreadCount->GetCount() == 0;
}

void ThreadManager::ShutdownThreads()
{
	ShouldShutdown.Flag();
	while (this->poThreadCount->GetCount() != 0)
	{
		NotifyAll();
	}
}

ThreadManager& ThreadManager::GetInstance()
{
	static ThreadManager inst;
	return inst;
}

