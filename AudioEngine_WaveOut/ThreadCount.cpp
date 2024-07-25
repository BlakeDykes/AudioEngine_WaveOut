#include "ThreadCount.h"

ThreadCount::ThreadCount(ThreadCountZeroFlag& zeroThreadsFlag)
	: mtx(), ActiveThreads(0), ThreadCountZero(zeroThreadsFlag)
{
}

void ThreadCount::Increment()
{
	std::lock_guard<std::mutex> m(this->mtx);
	++ActiveThreads;

	//Debug::out("Thread Count : % d\n", ActiveThreads);
}

void ThreadCount::Decrement()
{
	std::lock_guard<std::mutex> m(this->mtx);

	if (--ActiveThreads <= 0)
	{
		ActiveThreads = 0;
		ThreadCountZero.FlagOne();
	}

	//Debug::out("Thread Count : % d\n", ActiveThreads);
}

int ThreadCount::GetCount()
{
	std::lock_guard<std::mutex> m(this->mtx);
	return ActiveThreads;
}

