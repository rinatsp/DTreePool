#include "ThreadAffinity.h"


thread_local int ThreadAffinity::core = -1;


int ThreadAffinity::set_cpu(const unsigned short & core) const
{
	bool error = false;
	error = !SetThreadAffinityMask(GetCurrentThread(), 1 << core);
	return (!error) ? core : -1;
}

ThreadAffinity::ThreadAffinity() :
	counter(0),
	total_cores(std::max<unsigned short>(std::thread::hardware_concurrency(), 1))
{}


ThreadAffinity::~ThreadAffinity()
{
	
}

int ThreadAffinity::set_core() const
{
	const unsigned short new_core = counter++ % total_cores;
	const int result = this->set_cpu(new_core);
	core = result;
	if (result < 0) {
		printf("set_cpu error\n");
	}
	return result;
}

int ThreadAffinity::get_core_num() const
{
	return core;
}

int ThreadAffinity::get_total_cores() const
{
	return total_cores;
}
