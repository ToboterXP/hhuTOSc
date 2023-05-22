#ifndef __threadTest__
#define __threadTest__

#include "kernel/threads/Thread.h"
#include "lib/Semaphore.h"

class ThreadTest : public Thread {
private:
	static Semaphore test;
	int i;
public:
	ThreadTest(int a) : i(a)  {}
	void main();
};


#endif
