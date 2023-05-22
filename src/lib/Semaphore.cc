#include "Semaphore.h"

#include "kernel/Globals.h"

// 'Passieren': Warten auf das Freiwerden eines kritischen Abschnitts.
void Semaphore::acquire () {
	//access_lock.waitForAcquire();
	counter--;
	//kout <<"AB"<< counter << endl;
	if (counter <= 0 && lock.acquire()) { //lock spinlock on first thread before the semaphore is closed
		waitQueue.prepend(scheduler.get_active());
		//access_lock.release();
		scheduler.block();
	} else {
		acquiredList.prepend(scheduler.get_active());
	}
	//access_lock.release();
}

// 'Vreigeben': Freigeben des kritischen Abschnitts.
void Semaphore::release () {
	//access_lock.waitForAcquire();
	counter++;
	//kout << "BB"<<counter << endl;
	if (!waitQueue.is_empty()) {
		Thread* next = waitQueue.pop_last();
		acquiredList.prepend(next);
		scheduler.unblock(next);
	} else {
		lock.release(); //release lock if no further threads are waiting
	}

	//remove from acquired list
	auto current = acquiredList.get_first();
	while (current) {
		if (current->data == scheduler.get_active()) {
			acquiredList.remove(current);
			break;
		}
		current = current->GetNext();
	}

	//access_lock.release();
}


bool Semaphore::has_acquired(Thread * thread) {
	//access_lock.waitForAcquire();
	auto current = acquiredList.get_first();
	while (current) {
		if (current->data == thread) {
			access_lock.release();
			return true;
		}
		current = current->GetNext();
	}
	//access_lock.release();
	return false;
}

bool Semaphore::is_locked() {
	return lock.isLocked();
}
