#include "CircularQueue.h"

CircularQueue::CircularQueue()
	: Front(CIRCULAR_DATA_SIZE),
	Back(CIRCULAR_DATA_SIZE),
	Empty(true),
	Full(false),
	mtx(),
	Pusher(this)
{
}

bool CircularQueue::PushBack(QueueCommand* val)
{

	std::lock_guard<std::mutex> m(this->mtx);

	if (this->Front != this->Back || this->Empty == true)
	{
		this->Data[this->Back.GetIndex()] = val;
		this->Back++;

		this->Empty = false;

		if (this->Front == this->Back)
		{
			this->Full = true;
		}

		return true;
	}
	
	return false;
}

bool CircularQueue::PopFront(QueueCommand*& val)
{
	std::lock_guard<std::mutex> m(this->mtx);

	if (this->Front != this->Back || this->Full == true)
	{
		val = this->Data[this->Front.GetIndex()];

		this->Front++;

		this->Full = false;

		if (this->Front == this->Back)
		{
			this->Empty = true;
		}

		return true;
	}

	return false;
}

QueuePusher* CircularQueue::GetPusher()
{
	return &this->Pusher;
}

bool CircularQueue::IsFull()
{
	return Full;
}

bool CircularQueue::IsEmpty()
{
	return Empty;
}
