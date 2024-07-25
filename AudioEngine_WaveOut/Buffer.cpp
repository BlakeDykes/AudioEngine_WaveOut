#include "Buffer.h"

Buffer::Buffer(size_t size)
	: poBuffer(new unsigned char[size]()), pBuffPosition(poBuffer),
	TotalSize(size), CurrSize(0), Stat(Buffer::Status::Empty)
{
	assert(poBuffer);
}

Buffer::~Buffer()
{
	delete this->poBuffer;
	this->poBuffer = nullptr;
}

unsigned char* Buffer::GetRawBuffer()
{
	return this->poBuffer;
}

unsigned char* Buffer::GetCurrentPos()
{
	return this->pBuffPosition;
}

size_t Buffer::GetTotalSize() const
{
	return this->TotalSize;
}

size_t Buffer::GetCurrSize() const
{
	return this->CurrSize;
}

void Buffer::UpdateCurrentPos(const size_t s)
{
	if (s >= CurrSize)
	{
		SetEmpty();
	}
	else
	{
		this->CurrSize -= s;
		this->pBuffPosition += s;
	}
}

void Buffer::SetFilled(const size_t s)
{
	assert(s > 0);
	this->CurrSize = s;
	this->pBuffPosition = poBuffer;
	this->Stat = Buffer::Status::Filled;
}

void Buffer::SetEmpty()
{
	this->CurrSize = 0;
	this->pBuffPosition = poBuffer;
	this->Stat = Buffer::Status::Empty;
}

bool Buffer::IsEmpty() const
{
	return this->CurrSize <= 0;
}

size_t Buffer::TransferTo(Buffer*& pBuffer)
{
	return privTransferTo(pBuffer);
}

size_t Buffer::TransferFrom(const unsigned char* const pData, size_t size)
{
	return privTransferFrom(pData, size);
}

size_t Buffer::privTransferFrom(const unsigned char* const pData, size_t size)
{
	if (size == 0)
	{
		SetEmpty();
		return 0;
	}

	size_t transferSize = this->TotalSize < size ? this->TotalSize : size;

	memcpy_s(this->pBuffPosition, this->TotalSize,
		pData, transferSize);

	this->CurrSize = transferSize;
	this->Stat = Status::Filled;

	return transferSize;
}

size_t Buffer::privTransferTo(Buffer*& pBuffer)
{
	if (this->Stat == Status::Empty) return 0;

	size_t transferSize = pBuffer->TotalSize < this->CurrSize ? pBuffer->TotalSize : this->CurrSize;

	memcpy_s(pBuffer->pBuffPosition, pBuffer->TotalSize,
		this->pBuffPosition, transferSize);

	pBuffer->CurrSize = transferSize;
	pBuffer->Stat = Status::Filled;

	this->CurrSize -= transferSize;

	if (this->CurrSize == 0)
	{
		pBuffPosition = poBuffer;
		Stat = Status::Empty;
	}
	else
	{
		pBuffPosition += transferSize;
	}

	return transferSize;
}

Buffer::Status Buffer::GetStatus() const
{
	return this->Stat;
}
