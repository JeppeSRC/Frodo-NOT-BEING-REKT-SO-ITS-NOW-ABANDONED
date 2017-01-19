#include "input.h"
#include <memory>

bool		  Input::keys[FD_INPUT_MAX_KEYS];
bool		  Input::prevKeys[FD_INPUT_MAX_KEYS];
Window*		  Input::window = nullptr;

uint32  Input::mouseX = 0;
uint32  Input::mouseY = 0;
bool		  Input::mouseCaptured = false;

void Input::Init(Window* window) {
	Input::window = window;
	memset(prevKeys, 0, FD_INPUT_MAX_KEYS);
	memset(keys, 0, FD_INPUT_MAX_KEYS);
}

void Input::Update() {
	if (mouseCaptured) SetMousePos(window->GetWidth() >> 1, window->GetHeight() >> 1);
}

bool Input::IsKeyDown(byte key) {
	return keys[key];
}

bool Input::IsKeyDownOnce(byte key) {
	if (keys[key] && !prevKeys[key]) {
		prevKeys[key] = true;
		return true;
	}

	return false;
}

void Input::SetMousePos(uint32 x, uint32 y) {
	POINT p;
	p.x = x;
	p.y = y;

	ClientToScreen(window->GetHWND(), &p);

	SetCursorPos(p.x, p.y);
}
