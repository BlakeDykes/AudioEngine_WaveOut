#pragma once

#include "QueueCommand.h"
#include "CircularIndex.h"
#include "QueuePusher.h"

class CircularQueue
{
public:
	static const int CIRCULAR_DATA_SIZE = 1 << 8;

	CircularQueue();
	CircularQueue(const CircularQueue&) = delete;
	const CircularQueue& operator =(const CircularQueue&) = delete;
	~CircularQueue() = default;

	bool PushBack(QueueCommand* val);
	bool PopFront(QueueCommand*& val);

	QueuePusher* GetPusher();

	bool IsFull();
	bool IsEmpty();

private:
	QueueCommand* Data[CIRCULAR_DATA_SIZE];

	CircularIndex Front;
	CircularIndex Back;

	bool Empty;
	bool Full;

	std::mutex mtx;

	QueuePusher Pusher;
};