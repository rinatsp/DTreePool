#include "Lib.h"
#include "PoolDT.h"


int main()
{
	setlocale(LC_ALL, "Russian");
	int dt;
	cout << "¬ведите кол-во потоков" << endl;
	cin >> dt;
	PoolDT poll(0,dt, 3);	

	//boost::lockfree::queue<int, boost::lockfree::capacity<false>> q;
	rigtorp::MPMCQueue<int> q(400000);
	auto t1 = std::chrono::high_resolution_clock::now();
	poll.test();
	/*for (int i = 0; i < 20000000; i++)
	{
		q.push(5);
		q.pop(dt);
	}*/

	auto t2 = std::chrono::high_resolution_clock::now();
	auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
	std::chrono::duration<double, std::milli> fp_ms = t2 - t1;

	std::cout << "Time test " << fp_ms.count() << " ms, "
		<< "or " << int_ms.count() << " whole milliseconds\n";
	system("pause");
	return 0;
}