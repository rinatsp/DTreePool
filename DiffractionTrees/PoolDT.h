#pragma once
#include "Lib.h"
#include "Node.h"
#include "ThreadAffinity.h"
#include "Queue.h"
const size_t D = 1000000;
struct queue_lf {
	//boost::lockfree::queue<int , boost::lockfree::capacity<false>> queue;
	
	rigtorp::MPMCQueue<int> qu(D);
	
};

class PoolDT
{
	int thread_count;
	int count_queue = 0;
	int h_tree;
	atomic_int produser_count = 0 , consumer_count = 0;
	Node *tree;
	ThreadAffinity thread_aff_mg;
	std::vector<std::thread> workers;
public:
	
	void test();
	void push(int data, int  thread_id);
	void pop(int j, int thread_id);
	PoolDT();
	PoolDT(int depth,int thread_c, int h);
	~PoolDT();
};

