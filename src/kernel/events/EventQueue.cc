#include "EventQueue.h"

#include <kernel/Globals.h>


void EventQueue::PushEvent(Event event) {
	cpu.disable_int();

	events.prepend(event);
	eventCount++;
	//kout << events.first << " " << events.last << endl;

	cpu.enable_int();
}

Event EventQueue::PopEvent() {
	cpu.disable_int();

	Event ret = events.pop_last();
	eventCount--;
	//kout << events.first << " " << events.last << endl;

	cpu.enable_int();

	return ret;
}


Event EventQueue::PeekEvent() {
	if (events.is_empty()) return Event();
	return events.get_last()->data;
}

void EventQueue::Flush() {
	cpu.disable_int();

	while(!events.is_empty()) events.pop_last();
	eventCount = 0;

	cpu.enable_int();
}

bool EventQueue::IsEmpty() {
	cpu.disable_int();
	bool ret = events.is_empty();
	cpu.enable_int();

	return ret;
}

void EventQueue::Wait() {
	cpu.halt();
}
