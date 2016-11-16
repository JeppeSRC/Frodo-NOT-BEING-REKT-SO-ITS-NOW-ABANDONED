#pragma once

#include "event.h"
#include <math/vec2.h>

class EventMouse : public Event {
protected:
	EventMouse(FD_TYPE type) : Event(type) {}

public:
	EventMouse() : Event(FD_MOUSE_EVENT) {}
};

class EventMouseActionMove : public EventMouse {
private:
	ivec2 position;
	
public:
	EventMouseActionMove(ivec2 position) : EventMouse(FD_MOUSE_ACTION_MOVE) { this->position = position; }

	inline ivec2 GetPosition() const { return position; }
};

class EventMouseActionButton : public EventMouse {
public:
	enum {
		FD_LEFTBUTTON = 0,
		FD_MIDDLEBUTTON = 1,
		FD_RIGHTBUTTON = 2
	};

private:
	unsigned int button;
	bool pressed;
public:
	EventMouseActionButton(bool pressed, unsigned int button) : EventMouse(pressed ? FD_MOUSE_ACTION_BUTTON_PRESSED : FD_MOUSE_ACTION_BUTTON_RELEASED) { this->pressed = pressed; }

	inline unsigned int GetButton() const { return button; }
	inline bool IsPressed() const { return pressed; }
};