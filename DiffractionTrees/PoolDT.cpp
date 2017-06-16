#include "PoolDT.h"



void PoolDT::test()
{
	for (int i = 0; i < thread_count; ++i)
	{
		workers.push_back(std::thread([&, i]() {
			if (i < thread_count / 2) {
				if (thread_aff_mg.get_core_num() == -1)
				{
					thread_aff_mg.set_core();
				}
				for (int j = 0; j < 10000000; j++)
					push(5, i);
			}
			else {
				if (thread_aff_mg.get_core_num() == -1)
				{
					thread_aff_mg.set_core();
				}
				for (int j = 0; j < 10000000; j++)
					pop(i);
			}
		}));
	}
	std::for_each(workers.begin(), workers.end(), [](std::thread &t)
	{
		t.join();
	});

}

void PoolDT::push(int data,int  thread_id)
{
	int index_queue = tree->travelse(tree, 0, thread_id);
	queue[index_queue].queue.push(data);
}

void PoolDT::pop(int  thread_id)
{
	int dt = 5;
	int index_queue = tree->travelse(tree, 1, thread_id);
	queue[index_queue].queue.pop(dt);
}

PoolDT::PoolDT()
{
}

PoolDT::PoolDT(int depth, int thread_c, int h)
{
	thread_count = thread_c;
	count_queue = pow(2, h);
	h_tree = h;
	tree = new Node(depth, h);
	queue = new queue_lf[count_queue];
	
	tree->levelOrderPrint(tree);

}



PoolDT::~PoolDT()
{
}
