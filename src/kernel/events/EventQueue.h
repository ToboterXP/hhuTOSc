#ifndef _inc_EventQueue
#define _inc_EventQueue


#include <lib/List.h>
#include <kernel/events/Event.h>
#include "lib/SpinLock.h"

class EventQueue {
private:
	List<Event> events = List<Event>(Event(EventType::NONE)); //takes events that arrive through interrupts

	SpinLock lock;
public:

	int eventCount = 0;
	void PushEvent(Event event);
	Event PopEvent(); //if not event is available, returns a NONE event
	Event PeekEvent();
	void Flush();

	bool IsEmpty();

	void Wait(); //waits until the next potential event
};

#endif
