#include "CircularIndex.h"

CircularIndex::CircularIndex(int buffSize)
	:Index(0), Size(buffSize), Mask((unsigned int)(buffSize - 1))
{
}

int CircularIndex::operator++(int)
{
	// plus one then mask for wrapping to front
	this->Index = (this->Index + 1) & (int)this->Mask;

	return this->Index;
}

bool CircularIndex::operator==(const CircularIndex& c) const
{
	return this->Size == c.Size &&
		this->Index == c.Index;
}

bool CircularIndex::operator!=(const CircularIndex& c) const
{
	return this->Index != c.Index||
		this->Size != c.Size;
}

int CircularIndex::GetIndex() const
{
	return this->Index;
}

void CircularIndex::SetIndex(int i)
{
	this->Index = i;
}
