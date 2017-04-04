#pragma once


#include <fd.h>
#include "eventkayboard.h"
#include "eventmouse.h"
#include "eventwindow.h"
#include "eventdispatcher.h"

namespace FD {

class FDAPI EventListener {
public:
	EventListener() { EventDispatcher::AddListener(this); }
	~EventListener() { EventDispatcher::RemoveListener(this); }

	virtual bool OnEvent(const Event* event) { return false; }

	virtual bool OnEvent(const EventMouseActionMove* event) { return false; }
	virtual bool OnMouseActionMove(ivec2 position) { return false; }

	virtual bool OnEvent(const EventMouseActionButton* event) { return false; }
	virtual bool OnMouseActionButtonPressed(uint32 button) { return false; }
	virtual bool OnMouseActionButtonReleased(uint32 button) { return false; }
	virtual bool OnMouseActionButtonHold(uint32 button) { return false; }

	virtual bool OnEvent(const EventWindowActionResize* event) { return false; }
	virtual bool OnWindowActionResize(ivec2 size) { return false; }

	virtual bool OnEvent(const EventWindowActionMove* event) { return false; }
	virtual bool OnWindowActionMove(ivec2 position) { return false; }

	virtual bool OnEvent(const EventWindowState* event) { return false; }
	virtual bool OnWindowStateChanged(EventWindowState::FD_ACTION action) { return false; }

	virtual bool OnEvent(const EventKeyboardActionKey* event) { return false; }
	virtual bool OnKeyboardActionKeyPressed(uint32 key) { return false; }
	virtual bool OnKeyboardActionKeyReleased(uint32 key) { return false; }
	virtual bool OnKeyboardActionKeyHold(uint32 key) { return false; }
};


}