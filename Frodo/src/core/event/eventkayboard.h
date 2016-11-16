#pragma once

#include "event.h"

class EventKeyboard : public Event {
protected:
	EventKeyboard(FD_TYPE type) : Event(type) {}

public:
	EventKeyboard() : Event(FD_KEYBOARD_EVENT) {}
};

class EventKeyboardActionKey : public EventKeyboard {
private:
	unsigned int key;
	bool pressed;
public:
	EventKeyboardActionKey(bool pressed, unsigned int key) : EventKeyboard(pressed ? FD_KEYBOARD_ACTION_KEY_PRESSED : FD_KEYBOARD_ACTION_KEY_RELEASED) { this->key = key; this->pressed = pressed; }

	inline unsigned int GetKey() const { return key; }
	inline bool IsPressed() const { return pressed; }
};