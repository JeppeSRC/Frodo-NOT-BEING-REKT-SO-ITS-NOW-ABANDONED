#include "eventdispatcher.h"
#include "eventlistener.h"

namespace FD {

List<EventListener*> EventDispatcher::listeners;

void EventDispatcher::AddListener(EventListener* listener) {
	listeners.Push_back(listener);
}

void EventDispatcher::RemoveListener(EventListener* listener) {
	listeners.Remove(listener);
}

void EventDispatcher::DispatchEvent(const Event* event) {
	uint_t num = listeners.GetSize();

	for (uint_t i = 0; i < num; i++) {
		EventListener* listener = listeners[i];
		listener->OnEvent(event);

		bool ret = false;

		static const EventKeyboardActionKey* keyboardActionKey;
		static const EventWindowState* windowState;
		static const EventWindowActionMove* windowActionMove;
		static const EventWindowActionResize* windowActionResize;
		static const EventMouseActionMove* mouseActionMove;
		static const EventMouseActionButton* mouseActionButton;

		switch (event->GetEventType()) {
			case FD_MOUSE_ACTION_MOVE:
			case FD_MOUSE_ACTION_MOVE_ABSOLUTE:
			case FD_MOUSE_ACTION_MOVE_RELATIVE: {
				mouseActionMove = (const EventMouseActionMove*)event;
				listener->OnEvent(mouseActionMove);
				ivec2 pos = mouseActionMove->GetPosition();
				ret = listener->OnMouseActionMove(pos);
				switch (mouseActionMove->GetAction()) {
				case FD_ABSOLUTE:
					listener->OnMouseActionMoveAbsolute(pos);
						break;
				case FD_RELATIVE:
					listener->OnMouseActionMoveRelative(pos);
					break;
				}
				break;
			}
			case FD_MOUSE_ACTION_BUTTON_RELEASED:
			case FD_MOUSE_ACTION_BUTTON_PRESSED:
			case FD_MOUSE_ACTION_BUTTON_HOLD:
				mouseActionButton = (const EventMouseActionButton*)event;
				listener->OnEvent(mouseActionButton);
				switch (mouseActionButton->GetAction()) {
					case FD_PRESSED:
						ret = listener->OnMouseActionButtonPressed(mouseActionButton->GetButton());
						break;
					case FD_RELEASED:
						ret = listener->OnMouseActionButtonReleased(mouseActionButton->GetButton());
						break;
					case FD_HOLD:
						ret = listener->OnMouseActionButtonHold(mouseActionButton->GetButton());
						break;
				}
				break;

			case FD_WINDOW_ACTION_RESIZE:
				windowActionResize = (const EventWindowActionResize*)event;
				listener->OnEvent(windowActionResize);
				ret = listener->OnWindowActionResize(windowActionResize->GetSize());
				break;
			case FD_WINDOW_ACTION_MOVE:
				windowActionMove = (const EventWindowActionMove*)event;
				listener->OnEvent(windowActionMove);
				ret = listener->OnWindowActionMove(windowActionMove->GetPosition());
				break;

			case FD_WINDOW_STATE_MINIMIZED:
			case FD_WINDOW_STATE_MAXIMIZED:
			case FD_WINDOW_STATE_FOCUS_GAINED:
			case FD_WINDOW_STATE_FOCUS_LOST:
				windowState = (const EventWindowState*)event;
				listener->OnEvent(windowState);
				ret = listener->OnWindowStateChanged(windowState->GetAction());
				break;


			case FD_KEYBOARD_ACTION_KEY_RELEASED:
			case FD_KEYBOARD_ACTION_KEY_PRESSED:
			case FD_KEYBOARD_ACTION_KEY_HOLD:
				keyboardActionKey = (const EventKeyboardActionKey*)event;
				listener->OnEvent(keyboardActionKey);
				switch (keyboardActionKey->GetAction()) {
					case FD_PRESSED:
						ret = listener->OnKeyboardActionKeyPressed(keyboardActionKey->GetKey());
						break;
					case FD_RELEASED:
						ret = listener->OnKeyboardActionKeyReleased(keyboardActionKey->GetKey());
						break;
					case FD_HOLD:
						ret = listener->OnKeyboardActionKeyHold(keyboardActionKey->GetKey());
						break;
				}
				break;
		}
	}

	delete event;
}

}