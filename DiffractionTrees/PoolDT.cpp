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
					push(j, i);
			}
			else {
				if (thread_aff_mg.get_core_num() == -1)
				{
					thread_aff_mg.set_core();
				}
				for (int j = 0; j < 10000000 / thread_count; j++)
					pop(j,i);
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
	queue[index_queue].push(data);
	//cout << index_queue << " push" << endl;
}

void PoolDT::pop(int j,int  thread_id)
{
	int index_queue = tree->travelse(tree, 1, thread_id);
	queue[index_queue].pop(j);
	//queue[index_queue].queue.pop(j);
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
	tree->levelOrderPrint(tree);
	queue = new rigtorp::MPMCQueue<int>[count_queue];
	//tree->print2DUtil(tree,0); // вывод дерева на экран
}



PoolDT::~PoolDT()
{
}
