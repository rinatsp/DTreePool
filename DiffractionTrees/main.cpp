#include "Lib.h"
#include "PoolDT.h"


int main()
{
	PoolDT poll(0,8, 3);	

	//boost::lockfree::queue<int, boost::lockfree::capacity<false>> q;
	int dt = 5;
	//rigtorp::MPMCQueue<int> q(10000000);
	auto t1 = std::chrono::high_resolution_clock::now();
	poll.test();
	/*for (int i = 0; i < 10000000; i++)
		q.push(5);
	for (int i = 0; i < 10000000; i++)
		q.pop(dt);*/
	auto t2 = std::chrono::high_resolution_clock::now();
	auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
	std::chrono::duration<double, std::milli> fp_ms = t2 - t1;

	std::cout << "Time test " << fp_ms.count() << " ms, "
		<< "or " << int_ms.count() << " whole milliseconds\n";
	system("pause");
	return 0;
}