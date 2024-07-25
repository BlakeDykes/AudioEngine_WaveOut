#include "QueuePusher.h"
#include "CircularQueue.h"
#include "QueueCommand.h"

QueuePusher::QueuePusher(CircularQueue* queue)
	: pQueue(queue)
{
}

void QueuePusher::Push(QueueCommand* c)
{
	std::lock_guard<std::mutex> m(mtx);

	if (pQueue)
	{
		pQueue->PushBack(c);
	}
}

