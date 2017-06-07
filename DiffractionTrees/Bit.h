#pragma once
#include "Lib.h"
class Bit
{
	atomic_int bit;
public:
	void flip()
	{
		bit.fetch_xor(1);
	}
	Bit();
	~Bit();
};

