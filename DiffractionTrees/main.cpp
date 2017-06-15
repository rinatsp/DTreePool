#include "Lib.h"
#include "PoolDT.h"


int main()
{
	PoolDT* poll = new PoolDT(0,8,2);
	poll->test();
	system("pause");
	return 0;
}