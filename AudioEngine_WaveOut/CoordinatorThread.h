#pragma once

#include "ThreadBase.h"
#include "Buffer.h"

class FileThread;

class CoordinatorThread : public QueueThreadBase
{
public:
	static const unsigned int BUFFER_SIZE = 512 * 1024;

	CoordinatorThread() = delete;
	CoordinatorThread(FileThread& f);
	virtual ~CoordinatorThread();

	void PushTo(Buffer* pToBuffer);

protected:
	virtual void ThreadMain() override;

	void PullFileBufferIfReady();
	void SwapBuffersIfReady();
	void ProcessQueue();

	Buffer* poBufferA;
	Buffer* poBufferB;

	Buffer* pPullBuffer;
	Buffer* pPushBuffer;
	std::future<Buffer*&> fFileTransfer;

	FileThread& FileThreadRef;

	FileToCoordCompleteFlag& FileToCoordComplete;
};