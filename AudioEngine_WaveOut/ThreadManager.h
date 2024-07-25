#pragma once

#include "ThreadCount.h"
#include "ThreadFlags.h"
#include <vector>

class ThreadManager
{
public:
	static std::future<void>& GetShutdownFuture();
	static CVFlag& GetManagedCV();

	static void RegisterThreadStart();
	static void RegisterThreadEnd();

	static void ShutdownOnKeyPress();

private:
	ThreadManager();
	~ThreadManager();
	static ThreadManager& GetInstance();

	void ShutdownThreads();
	bool NotifyAll();

	ShouldShutdownFlag ShouldShutdown;
	ThreadCountZeroFlag ThreadCountZero;
	std::vector<CVFlag*> CVs;
	ThreadCount* poThreadCount;
};