#include "PoolDT.h"



void PoolDT::test()
{
	for (int i = 0; i < thread_count; ++i)
	{
		workers.push_back(std::thread([&, i]() {
			if (i%2 == 0) {
				if (thread_aff_mg.get_core_num() == -1)
				{
					thread_aff_mg.set_core();
				}
				for (int j = 0; j < 10000000 / thread_count; j++)
					push(5, i);
			}
			else {
				if (thread_aff_mg.get_core_num() == -1)
				{
					thread_aff_mg.set_core();
				}
				for (int j = 0; j < 10000000 / thread_count; j++)
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
	//queue[index_queue].queue.push(data);
	queue[index_queue].queue.enqueue(data);
	//cout << index_queue << " push" << endl;
}

void PoolDT::pop(int  thread_id)
{
	int dt = 5;
	int index_queue = tree->travelse(tree, 1, thread_id);
	//queue[index_queue].queue.pop(dt);
	queue[index_queue].queue.dequeue(dt);
	//cout << index_queue << " pop" << endl;
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
	//tree->print2DUtil(tree,0); // ����� ������ �� �����
}



PoolDT::~PoolDT()
{
}
