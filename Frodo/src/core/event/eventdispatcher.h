#pragma once

#include <fd.h>
#include <util/list.h>

#include "event.h"

class FDAPI EventDispatcher {
private:
	friend class EventListener;
private:
	static List<EventListener*> listeners;

public:

	static void AddListener(EventListener* listener);
	static void RemoveListener(EventListener* listener);

	static void DispatchEvent(const Event* event);
};
