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
		FD_BUTTON_LEFT,
		FD_BUTTON_RIGHT,
		FD_BUTTON_MIDDLE
	};

	enum FD_ACTION {
		FD_PRESSED,
		FD_RELEASED,
		FD_HOLD
	};

private:
	unsigned int button;
	FD_ACTION action;
public:
	EventMouseActionButton(FD_ACTION action, unsigned int button) : EventMouse(action == FD_PRESSED ? FD_MOUSE_ACTION_BUTTON_PRESSED : action == FD_RELEASED ? FD_MOUSE_ACTION_BUTTON_RELEASED : FD_MOUSE_ACTION_BUTTON_HOLD) { this->action = action; }

	inline unsigned int GetButton() const { return button; }
	inline FD_ACTION GetAction() const { return action; }
};