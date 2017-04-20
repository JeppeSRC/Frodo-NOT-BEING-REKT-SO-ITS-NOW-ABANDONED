#pragma once

#include "event.h"
#include <math/vec2.h>

namespace FD {

enum FD_MOUSE_BUTTON {
	FD_MOUSE_BUTTON0,
	FD_MOUSE_BUTTON1,
	FD_MOUSE_BUTTON2,
	FD_MOUSE_BUTTON4,
	FD_MOUSE_BUTTON5,
	FD_MOUSE_BUTTON6,
	FD_MOUSE_BUTTON7,

	FD_MOUSE_BUTTON_NUM,

	FD_MOUSE_BUTTON_LEFT = FD_MOUSE_BUTTON0,
	FD_MOUSE_BUTTON_RIGHT = FD_MOUSE_BUTTON1,
	FD_MOUSE_BUTTON_MIDDLE = FD_MOUSE_BUTTON2,
};

class EventMouse : public Event {
protected:
	EventMouse(FD_EVENT_TYPE type) : Event(type) {}

public:
	EventMouse() : Event(FD_MOUSE_EVENT) {}
};

class EventMouseActionMove : public EventMouse {
private:
	ivec2 position;

	FD_EVENT_ACTION action;

public:
	EventMouseActionMove(ivec2 position, FD_EVENT_ACTION action = FD_EVENT_ACTION_UNKNOWN) : EventMouse(action == FD_EVENT_ACTION_UNKNOWN ? FD_MOUSE_ACTION_MOVE : action == FD_ABSOLUTE ? FD_MOUSE_ACTION_MOVE_ABSOLUTE : action == FD_RELATIVE ? FD_MOUSE_ACTION_MOVE_RELATIVE : FD_EVENT_TYPE_UNKNOWN) { this->position = position; this->action = action; }

	inline ivec2 GetPosition() const { return position; }
	inline FD_EVENT_ACTION GetAction() const { return action; }
};

class EventMouseActionButton : public EventMouse {
private:
	uint32 button;
	FD_EVENT_ACTION action;
public:
	EventMouseActionButton(FD_EVENT_ACTION action, uint32 button) : EventMouse(action == FD_PRESSED ? FD_MOUSE_ACTION_BUTTON_PRESSED : action == FD_RELEASED ? FD_MOUSE_ACTION_BUTTON_RELEASED : FD_MOUSE_ACTION_BUTTON_HOLD) { this->action = action; }

	inline uint32 GetButton() const { return button; }
	inline FD_EVENT_ACTION GetAction() const { return action; }
};

}