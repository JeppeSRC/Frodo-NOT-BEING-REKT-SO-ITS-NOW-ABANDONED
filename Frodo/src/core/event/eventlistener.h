#pragma once


#include <fd.h>
#include "eventkayboard.h"
#include "eventmouse.h"
#include "eventwindow.h"

class FDAPI EventListener {
public:
	virtual bool OnEvent(const Event* event) = 0;

	virtual bool OnEvent(const EventMouseActionMove* event) { return false; }
	virtual bool OnEvent(const EventMouseActionButton* event) { return false; }

	virtual bool OnEvent(const EventWindowActionResize* event) { return false; }
	virtual bool OnEvent(const EventWindowActionMove* event) { return false; }
	virtual bool OnEvent(const EventWindowState* event) { return false; }
	virtual bool OnEvent(const EventWindowStateFocus* event) { return false; }

	virtual bool OnEvent(const EventKeyboardActionKey* event) { return false; }

};
