#include "input.h"
#include "event/eventdispatcher.h"
#include "event/eventkeyboard.h"
#include "event/eventmouse.h"

namespace FD {



inline static uint32 RemapDIK(byte dik) {
	switch (dik) {
	case DIK_0:
		return '0';
	case DIK_1:
		return '1';
	case DIK_2:
		return '2';
	case DIK_3:
		return '3';
	case DIK_4:
		return '4';
	case DIK_5:
		return '5';
	case DIK_6:
		return '6';
	case DIK_7:
		return '7';
	case DIK_8:
		return '8';
	case DIK_9:
		return '9';

	case DIK_ADD:
		return '+';
	case DIK_APOSTROPHE:
		return '\'';
	case DIK_BACK:
		return VK_BACK;
	case DIK_BACKSLASH:
		return '\\';
	case DIK_C:
		return 'C';
	case DIK_CAPITAL:
		return VK_CAPITAL;
	case DIK_COLON:
		return ':';
	case DIK_COMMA:
		return ',';
	case DIK_D:
		return 'D';
	case DIK_DECIMAL:
		return VK_DECIMAL;
	case DIK_DELETE:
		return VK_DELETE;
	case DIK_DIVIDE:
		return VK_DIVIDE;
	case DIK_DOWN:
		return VK_DOWN;
	case DIK_E:
		return 'E';
	case DIK_END:
		return VK_END;
	case DIK_EQUALS:
		return '=';
	case DIK_ESCAPE:
		return VK_ESCAPE;
	case DIK_F:
		return 'F';
	case DIK_F1:
		return VK_F1;
	case DIK_F2:
		return VK_F2;
	case DIK_F3:
		return VK_F3;
	case DIK_F4:
		return VK_F4;
	case DIK_F5:
		return VK_F5;
	case DIK_F6:
		return VK_F6; 
	case DIK_F7:
		return VK_F7;
	case DIK_F8:
		return VK_F8;
	case DIK_F9:
		return VK_F9;
	case DIK_F10:
		return VK_F10;
	case DIK_F11:
		return VK_F11;
	case DIK_F12:
		return VK_F12;
	case DIK_G:
		return 'G';
	case DIK_GRAVE:
		return '`';
	case DIK_H:
		return 'H';
	case DIK_HOME:
		return VK_HOME;
	case DIK_I:
		return 'I';
	case DIK_INSERT:
		return VK_INSERT;
	case DIK_J:
		return 'J';
	case DIK_K:
		return 'K';
	case DIK_L:
		return 'L';
	case DIK_LBRACKET:
		return DIK_LBRACKET;
	case DIK_LCONTROL:
		return VK_LCONTROL;
	case DIK_LEFT:
		return VK_LEFT;
	case DIK_LMENU:
		return VK_LMENU;
	case DIK_LSHIFT:
		return VK_LSHIFT;
	case DIK_LWIN:
		return VK_LWIN;
	case DIK_M:
		return 'M';
	case DIK_MINUS:
		return '-';
	case DIK_MULTIPLY:
		return '*';
	case DIK_MUTE:
		return VK_VOLUME_MUTE;
	case DIK_N:
		return 'N';
	case DIK_NUMLOCK:
		return VK_NUMLOCK;
	case DIK_NUMPAD0:
		return VK_NUMPAD0;
	case DIK_NUMPAD1:
		return VK_NUMPAD1;
	case DIK_NUMPAD2:
		return VK_NUMPAD2;
	case DIK_NUMPAD3:
		return VK_NUMPAD3;
	case DIK_NUMPAD4:
		return VK_NUMPAD4;
	case DIK_NUMPAD5:
		return VK_NUMPAD5;
	case DIK_NUMPAD6:
		return VK_NUMPAD6;
	case DIK_NUMPAD7:
		return VK_NUMPAD7;
	case DIK_NUMPAD8:
		return VK_NUMPAD8;
	case DIK_NUMPAD9:
		return VK_NUMPAD9;
	case DIK_NUMPADCOMMA:
		return ',';
	case DIK_NUMPADENTER:
		return 0;
	}
}

void Input::HandleMouseEvents() {
	static bool buttons[FD_MOUSE_BUTTON_NUM]{ 0 };

	DIMOUSESTATE2 m;

	mouse.device->GetDeviceState(sizeof(DIMOUSESTATE2), &m);

	Event* e = nullptr;

	for (uint_t i = 0; i < 8; i++) {
		if (m.rgbButtons[i] & 0x80) {
			if (!buttons[i]) {
				e = new EventMouseActionButton(FD_PRESSED, (uint32)i);
				buttons[i] = true;
				EventDispatcher::DispatchEvent(e);
			}
		} else {
			if (buttons[i]) {
				e = new EventMouseActionButton(FD_RELEASED, (uint32)i);
				buttons[i] = false;
				EventDispatcher::DispatchEvent(e);
			}
		}
	}

	if (m.lX != 0 || m.lY != 0) {
		e = new EventMouseActionMove(ivec2(m.lX, m.lY), FD_RELATIVE);
		EventDispatcher::DispatchEvent(e);

		mouse.position.x += m.lX;
		mouse.position.y += m.lY;

		if (mouse.position.x < 0) mouse.position.x = 0;
		else if (mouse.position.x > window->GetWidth()) mouse.position.x = window->GetWidth();

		if (mouse.position.y < 0) mouse.position.y = 0;
		else if (mouse.position.y > window->GetHeight()) mouse.position.y = window->GetHeight();

		e = new EventMouseActionMove(mouse.position, FD_ABSOLUTE);
		EventDispatcher::DispatchEvent(e);
	}
}

void Input::HandleKeyboardEvents() {
	static bool keys[256];
	static byte k[256];

	keyboard.device->GetDeviceState(256, k);

	memcpy(keyboard.k, k, 256);

	keyboard.modifiers = 0;

	bool ctrl = false;

	if (k[FD_KEY_LSHIFT]) {
		keyboard.modifiers |= FD_MODIFIER_LSHIFT | FD_MODIFIER_SHIFT;
	}
	if (k[FD_KEY_RSHIFT]) {
		keyboard.modifiers |= FD_MODIFIER_RSHIFT | FD_MODIFIER_SHIFT;
	}
	if (k[FD_KEY_LCONTROL]) {
		keyboard.modifiers |= FD_MODIFIER_LCONTROL | FD_MODIFIER_CONTROL;
		ctrl = true;
	}
	if (k[FD_KEY_RCONTROL]) {
		keyboard.modifiers |= FD_MODIFIER_RCONTROL | FD_MODIFIER_CONTROL;
		ctrl = true;
	}
	if (k[FD_KEY_LMENU]) {
		keyboard.modifiers |= FD_MODIFIER_LALT | FD_MODIFIER_ALT | (ctrl ? FD_MODIFIER_ALTGR : 0);
	}
	if (k[FD_KEY_RMENU]) {
		keyboard.modifiers |= FD_MODIFIER_RALT | FD_MODIFIER_ALT | (ctrl ? FD_MODIFIER_ALTGR : 0);
	}

	Event* e = nullptr;

	
	for (uint_t i = 0; i < 256; i++) {
		if (k[i] & 0x80) {
			if (!keys[i]) {
				e = new EventKeyboardActionKey(FD_PRESSED, (FD_KEY)i);
				keys[i] = true;
				EventDispatcher::DispatchEvent(e);
			}
		} else {
			if (keys[i]) {
				e = new EventKeyboardActionKey(FD_RELEASED, (FD_KEY)i);
				keys[i] = false;
				EventDispatcher::DispatchEvent(e);
			}
		}
	}


	/*for (uint_t i = 0; i < 256; i++)
		if (k[i] & 0x80)
			FD_DEBUG("Key: %u", i);*/
}

void Input::UpdateInputAndDispatchEvents() {
	if (mouse.device && mouse.acquired) HandleMouseEvents();
	if (keyboard.device && keyboard.acquired) HandleKeyboardEvents();
}

}