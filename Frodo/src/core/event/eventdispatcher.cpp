#include "eventdispatcher.h"

List<EventListener*> EventDispatcher::listeners;

void EventDispatcher::AddListener(EventListener* listener) {
	listeners.Push_back(listener);
}

void EventDispatcher::RemoveListener(EventListener* listener) {
	listeners.Remove(listener);
}

void EventDispatcher::DispatchEvent(const Event* event) {
	size_t num = listeners.GetSize();

	for (size_t i = 0; i < num; i++) {
		EventListener* listener = listeners[i];
		listener->OnEvent(event);

		bool ret = false;

		switch(event->GetEventType()) {
		case Event::FD_MOUSE_ACTION_MOVE:
			ret = listener->OnEvent((const EventMouseActionMove*)event);
			break;
		case Event::FD_MOUSE_ACTION_BUTTON_RELEASED:
		case Event::FD_MOUSE_ACTION_BUTTON_PRESSED:
			ret = listener->OnEvent((const EventMouseActionButton*)event);
			break;

		case Event::FD_WINDOW_ACTION_RESIZE:
			ret = listener->OnEvent((const EventWindowActionResize*)event);
			break;
		case Event::FD_WINDOW_ACTION_MOVE:
			ret = listener->OnEvent((const EventWindowActionMove*)event);
			break;
		case Event::FD_WINDOW_STATE_MINIMIZED:
		case Event::FD_WINDOW_STATE_MAXIMIZED:
			ret = listener->OnEvent((const EventWindowState*)event);
			break;
		case Event::FD_WINDOW_STATE_FOCUS_GAINED:
		case Event::FD_WINDOW_STATE_FOCUS_LOST:
			ret = listener->OnEvent((const EventWindowStateFocus*)event);
			break;

		case Event::FD_KEYBOARD_ACTION_KEY_RELEASED:
		case Event::FD_KEYBOARD_ACTION_KEY_PRESSED:
			ret = listener->OnEvent((const EventKeyboardActionKey*)event);
			break;
		}
	}

	delete event;
}