#include "EventQueue.h"

#include <kernel/Globals.h>


void EventQueue::PushEvent(Event event) {
	lock.waitForAcquire();

	events.prepend(event);
	eventCount++;

	if (eventCount > 10) events.pop_last();

	lock.release();
}

Event EventQueue::PopEvent() {
	if (eventCount == 0) return Event();

	lock.waitForAcquire();

	Event ret = events.pop_last();
	if (eventCount > 0) eventCount--;
	//kout << events.first << " " << events.last << endl;
	lock.release();

	return ret;
}


Event EventQueue::PeekEvent() {
	if (events.is_empty()) return Event();
	return events.get_last()->data;
}

void EventQueue::Flush() {
	lock.waitForAcquire();

	while(!events.is_empty()) events.pop_last();
	eventCount = 0;

	lock.release();
}

bool EventQueue::IsEmpty() {
	bool ret = events.is_empty();

	return ret;
}

void EventQueue::Wait() {
	cpu.halt();
}
