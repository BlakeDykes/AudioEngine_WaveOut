#pragma once

class CircularIndex
{
public:
	CircularIndex() = delete;
	CircularIndex(int buffSize);

	CircularIndex(const CircularIndex&) = delete;
	const CircularIndex& operator = (const CircularIndex&) = delete;
	~CircularIndex() = default;

	int operator++(int);
	bool operator==(const CircularIndex& c) const;
	bool operator!=(const CircularIndex& c) const;

	int GetIndex() const;
	void SetIndex(int i);

private:
	int Index;
	int Size;
	unsigned int Mask;

};