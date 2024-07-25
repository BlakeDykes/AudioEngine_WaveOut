#pragma once

#include "ThreadCount.h"
#include "ThreadFlags.h"

class ThreadManager
{
public:
	static std::future<void>& GetShutdownFuture();

	static void RegisterThreadStart();
	static void RegisterThreadEnd();

	static void ShutdownOnKeyPress();

private:
	ThreadManager();
	~ThreadManager();
	static ThreadManager& GetInstance();

	void ShutdownThreads();

	ShouldShutdownFlag ShouldShutdown;
	ThreadCountZeroFlag ThreadCountZero;
	ThreadCount* poThreadCount;
};