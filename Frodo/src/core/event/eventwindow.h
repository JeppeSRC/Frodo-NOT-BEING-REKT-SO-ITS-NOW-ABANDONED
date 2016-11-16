#pragma once

#include "event.h"
#include <math/vec2.h>

class EventWindow : public Event {
protected:
	EventWindow(FD_TYPE type) : Event(type) {}

public:
	EventWindow() : EventWindow(FD_WINDOW_EVENT) {}
};

class EventWindowActionResize : public EventWindow {
private:
	ivec2 size;

public:
	EventWindowActionResize(ivec2 size) : EventWindow(FD_WINDOW_ACTION_RESIZE) { this->size = size; }

	inline ivec2 GetSize() const { return size; }
};

class EventWindowActionMove : public EventWindow {
private:
	ivec2 position;

public:
	EventWindowActionMove(ivec2 position) : EventWindow(FD_WINDOW_ACTION_MOVE) { this->position = position; }

	inline ivec2 GetPosition() const { return position; }
};

class EventWindowState : public EventWindow {
private:
	bool maximize;

public:
	EventWindowState(bool maximize) : EventWindow(maximize ? FD_WINDOW_STATE_MAXIMIZED : FD_WINDOW_STATE_MINIMIZED) { this->maximize = maximize; }

	inline bool IsMaximized() const { return maximize; }
};

class EventWindowStateFocus : public EventWindow {
private:
	bool gained;

public:
	EventWindowStateFocus(bool gained) : EventWindow(gained ? FD_WINDOW_STATE_FOCUS_GAINED : FD_WINDOW_STATE_FOCUS_LOST) { this->gained = gained; }

	inline bool GainedFocus() const { return gained; }
};