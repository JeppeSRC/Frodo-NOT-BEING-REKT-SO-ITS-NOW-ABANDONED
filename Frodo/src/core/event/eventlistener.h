#pragma once


#include <fd.h>
#include "eventkeyboard.h"
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
	virtual bool OnMouseActionMoveAbsolute(ivec2 position) { return false; }
	virtual bool OnMouseActionMoveRelative(ivec2 position) { return false; }

	virtual bool OnEvent(const EventMouseActionButton* event) { return false; }
	virtual bool OnMouseActionButtonPressed(uint32 button) { return false; }
	virtual bool OnMouseActionButtonReleased(uint32 button) { return false; }
	virtual bool OnMouseActionButtonHold(uint32 button) { return false; }

	virtual bool OnEvent(const EventWindowActionResize* event) { return false; }
	virtual bool OnWindowActionResize(ivec2 size) { return false; }

	virtual bool OnEvent(const EventWindowActionMove* event) { return false; }
	virtual bool OnWindowActionMove(ivec2 position) { return false; }

	virtual bool OnEvent(const EventWindowState* event) { return false; }
	virtual bool OnWindowStateChanged(FD_EVENT_ACTION action) { return false; }

	virtual bool OnEvent(const EventKeyboardActionKey* event) { return false; }
	virtual bool OnKeyboardActionKeyPressed(FD_KEY key) { return false; }
	virtual bool OnKeyboardActionKeyReleased(FD_KEY key) { return false; }
	virtual bool OnKeyboardActionKeyHold(FD_KEY key) { return false; }
};


}