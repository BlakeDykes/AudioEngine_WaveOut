#pragma once

struct Buffer
{
public:
	enum class Status
	{
		Filled,
		Empty
	};

	Buffer(size_t size);

	Buffer() = delete;
	Buffer(const Buffer&) = delete;
	Buffer& operator = (const Buffer&) = delete;
	~Buffer();

	unsigned char* GetRawBuffer();
	unsigned char* GetCurrentPos();
	void UpdateCurrentPos(const size_t moveDist);
	size_t GetTotalSize() const;
	size_t GetCurrSize() const;

	void SetFilled(const size_t s);
	void SetEmpty();
	bool IsEmpty() const;

	size_t TransferTo(Buffer*& pBuffer);
	size_t TransferFrom(const unsigned char* const pData, size_t size);

	Status GetStatus() const;

private:
	size_t privTransferTo(Buffer*& pBuffer);
	size_t privTransferFrom(const unsigned char* const pData, size_t size);

	unsigned char* poBuffer;
	unsigned char* pBuffPosition;

	size_t TotalSize;
	size_t CurrSize;
	Status Stat;
};