#include "ThreadTest.h"

#include "kernel/Globals.h"

Semaphore ThreadTest::test (1);

void ThreadTest::main() {
	kout.AcquireGraphicsLock();
	kout << "TID "<<tid<<" acquired graphics lock"<<endl;
	for (int j = 0; j<5; j++) {
		kout << j << " Thread: " << i << endl;
	}
	kout << "TID "<<tid<<" releasing graphics lock"<<endl;
	kout.ReleaseGraphicsLock();
}
