#include "CoordinatorThread.h"
#include "FileThread.h"
#include "QC_WaveFill.h"
#include "Wave.h"

CoordinatorThread::CoordinatorThread(FileThread& f)
	: QueueThreadBase("----- COORD ----"),
	poBufferA(new Buffer(CoordinatorThread::BUFFER_SIZE)),
	poBufferB(new Buffer(CoordinatorThread::BUFFER_SIZE)),
	pPullBuffer(poBufferA),
	pPushBuffer(poBufferB),
	fFileTransfer(f.GetTransferFuture()),
	FileThreadRef(f),
	FileToCoordComplete(std::ref(f.GetTransferCompleteCV()))
{
}

CoordinatorThread::~CoordinatorThread()
{
	if (fFileTransfer.valid() && fFileTransfer.wait_for(std::chrono::milliseconds(0)) != std::future_status::timeout)
	{
		fFileTransfer.get();
	}

	delete poBufferA;
	delete poBufferB;
}

void CoordinatorThread::ThreadMain()
{
	while (!ShouldShutdown())
	{
		PullFileBufferIfReady();

		SwapBuffersIfReady();
		
		ProcessQueue();
	}
}

void CoordinatorThread::PullFileBufferIfReady()
{
	if (!this->fFileTransfer.valid())
	{
		fFileTransfer = FileThreadRef.GetTransferFuture();
	}

	if (this->pPullBuffer->GetStatus() == Buffer::Status::Empty
		&& this->fFileTransfer.wait_for(0s) == std::future_status::ready)
	{
		Buffer* pFile = fFileTransfer.get();

		if (pFile)
		{
			/*size_t transferSize = */pFile->TransferTo(this->pPullBuffer);

			//Debug::out("File -> Coord Pull size - %d\n", transferSize);

			FileToCoordComplete.cv.notify_one();
		}
	}
}

void CoordinatorThread::SwapBuffersIfReady()
{
	if (!pPullBuffer->IsEmpty() && pPushBuffer->IsEmpty())
	{
		Buffer* pTemp = pPullBuffer;
		pPullBuffer = pPushBuffer;
		pPushBuffer = pTemp;

		//Debug::out("Push/Pull queues swapped\n");
	}
}

void CoordinatorThread::ProcessQueue()
{
	QueueCommand* pCurCommand = nullptr;

	if (!pPushBuffer->IsEmpty() && this->InQueue.PopFront(pCurCommand))
	{
		//size_t curSize = pPushBuffer->GetCurrSize();

		pCurCommand->Initialize((QueueBaseThread*)this);

		pCurCommand->Execute();

		while (this->pPushBuffer->GetCurrSize() > 0 && this->InQueue.PopFront(pCurCommand))
		{
			pCurCommand->Initialize((QueueBaseThread*)this);

			pCurCommand->Execute();
		}

		if (pPushBuffer->IsEmpty())
		{
			//Debug::out("Coord push buffer -> Wave complete complete\n");
		}
	}
}

void CoordinatorThread::PushTo(Buffer* pToBuffer)
{
	this->pPushBuffer->TransferTo(pToBuffer);
}

