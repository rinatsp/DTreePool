#pragma once
#include "Lib.h"
class PoolDT
{
	Node tree;
	ThreadAffinity thread_aff_mg;
	boost::lockfree::queue<int> queue;
public:
	void push(int data);
	void pop();
	PoolDT();
	~PoolDT();
};

