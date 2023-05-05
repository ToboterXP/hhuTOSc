#ifndef _inc_Event
#define _inc_Event

#include "devices/Keyboard.h"

enum class EventType {
	NONE,
	KEY_DOWN,
	KEY_UP
};

class KeyEvent {
public:
	bool has_key = false;
	uint8_t code;
	Key key;
};

class Event {
public:
	EventType type = EventType::NONE;
	KeyEvent keyEvent;

	Event(EventType type) {
		this->type = type;
	}

	Event() {}
};

#endif
