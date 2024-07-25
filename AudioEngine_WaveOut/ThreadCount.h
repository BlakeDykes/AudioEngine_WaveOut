#pragma once

#include "ThreadFlags.h"

class ThreadCount
{
public:
	ThreadCount() = delete;
	ThreadCount(ThreadCountZeroFlag& zeroThreadsFlag);

	ThreadCount(const ThreadCount&) = default;
	ThreadCount& operator =(const ThreadCount&) = default;
	~ThreadCount() = default;

	void Increment();
	void Decrement();

	int GetCount();

private:
	std::mutex mtx;
	int ActiveThreads;

	ThreadCountZeroFlag& ThreadCountZero;
};