#include "Lib.h"
#include "PoolDT.h"


int main()
{
	PoolDT poll(0, 150, 3);	



	auto t1 = std::chrono::high_resolution_clock::now();
	poll.test();
	auto t2 = std::chrono::high_resolution_clock::now();
	auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
	std::chrono::duration<double, std::milli> fp_ms = t2 - t1;

	std::cout << "Time test " << fp_ms.count() << " ms, "
		<< "or " << int_ms.count() << " whole milliseconds\n";
	system("pause");
	return 0;
}