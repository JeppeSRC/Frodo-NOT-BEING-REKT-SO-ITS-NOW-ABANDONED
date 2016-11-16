#pragma once

#include <fd.h>
#include <util/list.h>

#include "eventlistener.h"

class FDAPI EventDispatcher {
private:
	static List<EventListener*> listeners;

public:

	static void AddListener(EventListener* listener);
	static void RemoveListener(EventListener* listener);

	static void DispatchEvent(const Event* event);
};
