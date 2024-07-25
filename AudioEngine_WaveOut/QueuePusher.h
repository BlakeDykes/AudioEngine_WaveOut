#pragma once

class CircularQueue;
class QueueCommand;

class QueuePusher
{
public:
	QueuePusher() = delete;
	QueuePusher(CircularQueue* queue);

	QueuePusher(const QueuePusher& q) = delete;
	QueuePusher& operator=(const QueuePusher& q) = delete;
	~QueuePusher() = default;

	void Push(QueueCommand* c);

private:
	std::mutex mtx;
	CircularQueue* pQueue;
};