#ifndef _inc_EventQueue
#define _inc_EventQueue


#include <lib/List.h>
#include <kernel/events/Event.h>

class EventQueue {
private:
	List<Event> events = List<Event>(Event(EventType::NONE)); //takes events that arrive through interrupts

public:

	int eventCount = 0;
	void PushEvent(Event event);
	Event PopEvent();
	Event PeekEvent();
	void Flush();

	bool IsEmpty();

	void Wait();
};

#endif
