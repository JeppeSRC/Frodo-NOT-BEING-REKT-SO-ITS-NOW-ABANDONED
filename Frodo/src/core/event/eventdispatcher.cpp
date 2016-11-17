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

		static const EventKeyboardActionKey* keyboardActionKey;
		static const EventWindowState* windowState;
		static const EventWindowActionMove* windowActionMove;
		static const EventWindowActionResize* windowActionResize;
		static const EventMouseActionMove* mouseActionMove;
		static const EventMouseActionButton* mouseActionButton;

		switch(event->GetEventType()) {
		case Event::FD_MOUSE_ACTION_MOVE:
			mouseActionMove = (const EventMouseActionMove*)event;
			listener->OnEvent(mouseActionMove);
			ret = listener->OnMouseActionMove(mouseActionMove->GetPosition());
			break;
		case Event::FD_MOUSE_ACTION_BUTTON_RELEASED:
		case Event::FD_MOUSE_ACTION_BUTTON_PRESSED:
		case Event::FD_MOUSE_ACTION_BUTTON_HOLD:
			mouseActionButton = (const EventMouseActionButton*)event;
			listener->OnEvent(mouseActionButton);
			switch (mouseActionButton->GetAction()) {
			case EventMouseActionButton::FD_PRESSED:
				ret = listener->OnMouseActionButtonPressed(mouseActionButton->GetButton());
				break;
			case EventMouseActionButton::FD_RELEASED:
				ret = listener->OnMouseActionButtonReleased(mouseActionButton->GetButton());
				break;
			case EventMouseActionButton::FD_HOLD:
				ret = listener->OnMouseActionButtonHold(mouseActionButton->GetButton());
				break;
			}
			break;

		case Event::FD_WINDOW_ACTION_RESIZE:
			windowActionResize = (const EventWindowActionResize*)event;
			listener->OnEvent(windowActionResize);
			ret = listener->OnWindowActionResize(windowActionResize->GetSize());
			break;
		case Event::FD_WINDOW_ACTION_MOVE:
			windowActionMove = (const EventWindowActionMove*)event;
			listener->OnEvent(windowActionMove);
			ret = listener->OnWindowActionMove(windowActionMove->GetPosition());
			break;

		case Event::FD_WINDOW_STATE_MINIMIZED:
		case Event::FD_WINDOW_STATE_MAXIMIZED:
		case Event::FD_WINDOW_STATE_FOCUS_GAINED:
		case Event::FD_WINDOW_STATE_FOCUS_LOST:
			windowState = (const EventWindowState*)event;
			listener->OnEvent(windowState);
			ret = listener->OnWindowStateChanged(windowState->GetAction());
			break;
			

		case Event::FD_KEYBOARD_ACTION_KEY_RELEASED:
		case Event::FD_KEYBOARD_ACTION_KEY_PRESSED:
		case Event::FD_KEYBOARD_ACTION_KEY_HOLD:
			keyboardActionKey = (const EventKeyboardActionKey*)event;
			listener->OnEvent(keyboardActionKey);
			switch(keyboardActionKey->GetAction()){
			case EventKeyboardActionKey::FD_PRESSED:
				ret = listener->OnKeyboardActionKeyPressed(keyboardActionKey->GetKey());
				break;
			case EventKeyboardActionKey::FD_RELEASED:
				ret = listener->OnKeyboardActionKeyReleased(keyboardActionKey->GetKey());
				break;
				case EventKeyboardActionKey::FD_HOLD:
					ret = listener->OnKeyboardActionKeyHold(keyboardActionKey->GetKey());
					break;
			}
			break;
		}
	}

	delete event;
}