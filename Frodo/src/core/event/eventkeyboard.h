#pragma once

#include "event.h"
#include "fd_key.h"

namespace FD {

class EventKeyboard : public Event {
protected:
	EventKeyboard(FD_EVENT_TYPE type) : Event(type) {}

public:
	EventKeyboard() : Event(FD_KEYBOARD_EVENT) {}
};

class EventKeyboardActionKey : public EventKeyboard {
private:
	FD_KEY key;
	FD_EVENT_ACTION action;
public:
	EventKeyboardActionKey(FD_EVENT_ACTION action, FD_KEY key) : EventKeyboard(action == FD_PRESSED ? FD_KEYBOARD_ACTION_KEY_PRESSED : action == FD_RELEASED ? FD_KEYBOARD_ACTION_KEY_RELEASED : FD_KEYBOARD_ACTION_KEY_HOLD) { this->key = key; this->action = action; }

	inline FD_KEY GetKey() const { return key; }
	inline FD_EVENT_ACTION GetAction() const { return action; }
};

}