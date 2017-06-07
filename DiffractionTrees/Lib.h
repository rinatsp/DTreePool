#pragma once
#include <atomic>
#include <thread>
#include <queue>
#include <windows.h>
#include <boost/lockfree/queue.hpp>
#include "Node.h"
#include "ThreadAffinity.h"
#include "Bit.h"
#include "ArrayBits.h"


using namespace std;
