#include "PoolDT.h"



void PoolDT::test()
{
	for (int i = 0; i < 10; ++i)
	{
		workers.push_back(std::thread([&, i]() {
			thread_aff_mg.set_core();
			push(5, 1);
		}));
	}

}

void PoolDT::push(int data,int  thread_id)
{
	int index_queue = ( thread_aff_mg.get_total_cores() *  ( tree->travelse(tree, 0, thread_id) % ( count_queue / thread_aff_mg.get_total_cores() ))) + thread_aff_mg.get_core_num();
	queue[index_queue].queue.push(data);
	cout << index_queue << endl;
}

void PoolDT::pop()
{
}

PoolDT::PoolDT()
{
}

PoolDT::PoolDT(int depth, int thread_c, int h)
{
	thread_count = thread_count;
	count_queue = pow(2, h);
	h_tree = h;
	tree = new Node(depth, h);
	queue = new queue_lf[count_queue];
	
	tree->levelOrderPrint(tree);

}



PoolDT::~PoolDT()
{
}
