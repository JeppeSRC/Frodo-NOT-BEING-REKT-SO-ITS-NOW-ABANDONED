#pragma once


#include <fd.h>
#include "eventkayboard.h"
#include "eventmouse.h"
#include "eventwindow.h"

class FDAPI EventListener {
public:
	virtual bool OnEvent(const Event* event) { return false;  }

	virtual bool OnEvent(const EventMouseActionMove* event) { return false; }
	virtual bool OnMouseActionMove(ivec2 position) { return false; }

	virtual bool OnEvent(const EventMouseActionButton* event) { return false; }
	virtual bool OnMouseActionButtonPressed(unsigned int button) { return false; }
	virtual bool OnMouseActionButtonReleased(unsigned int button) { return false; }
	virtual bool OnMouseActionButtonHold(unsigned int button) { return false; }

	virtual bool OnEvent(const EventWindowActionResize* event) { return false; }
	virtual bool OnWindowActionResize(ivec2 size) { return false; }

	virtual bool OnEvent(const EventWindowActionMove* event) { return false; }
	virtual bool OnWindowActionMove(ivec2 position) { return false; }

	virtual bool OnEvent(const EventWindowState* event) { return false; }
	virtual bool OnWindowStateChanged(EventWindowState::FD_ACTION action) { return false; }


	virtual bool OnEvent(const EventKeyboardActionKey* event) { return false; }
	virtual bool OnKeyboardActionKeyPressed(unsigned int key) { return false; }
	virtual bool OnKeyboardActionKeyReleased(unsigned int key) { return false; }
	virtual bool OnKeyboardActionKeyHold(unsigned int key) { return false; }
};
