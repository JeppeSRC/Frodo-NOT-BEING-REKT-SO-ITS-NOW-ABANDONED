#pragma once

#include "event.h"

namespace FD {

class EventKeyboard : public Event {
protected:
	EventKeyboard(FD_TYPE type) : Event(type) {}

public:
	EventKeyboard() : Event(FD_KEYBOARD_EVENT) {}
};

class EventKeyboardActionKey : public EventKeyboard {
public:
	enum FD_ACTION {
		FD_PRESSED,
		FD_RELEASED,
		FD_HOLD
	};
private:
	uint32 key;
	FD_ACTION action;
public:
	EventKeyboardActionKey(FD_ACTION action, uint32 key) : EventKeyboard(action == FD_PRESSED ? FD_KEYBOARD_ACTION_KEY_PRESSED : action == FD_RELEASED ? FD_KEYBOARD_ACTION_KEY_RELEASED : FD_KEYBOARD_ACTION_KEY_HOLD) { this->key = key; this->action = action; }

	inline uint32 GetKey() const { return key; }
	inline uint32 GetAction() const { return action; }
};

}