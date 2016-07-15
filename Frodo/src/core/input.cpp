#include "input.h"
#include <memory>

bool		  Input::keys[FD_INPUT_MAX_KEYS];
bool		  Input::prevKeys[FD_INPUT_MAX_KEYS];
Window*		  Input::window = nullptr;

unsigned int  Input::mouseX = 0;
unsigned int  Input::mouseY = 0;
bool		  Input::mouseCaptured = false;

void Input::Init(Window* window) {
	Input::window = window;
	memset(prevKeys, 0, FD_INPUT_MAX_KEYS);
	memset(keys, 0, FD_INPUT_MAX_KEYS);
}

void Input::Update() {
	if (mouseCaptured) SetMousePos(window->GetWidth() >> 1, window->GetHeight() >> 1);
}

bool Input::IsKeyDown(unsigned char key) {
	return keys[key];
}

bool Input::IsKeyDownOnce(unsigned char key) {
	if (keys[key] && !prevKeys[key]) {
		prevKeys[key] = true;
		return true;
	}

	return false;
}

void Input::SetMousePos(unsigned int x, unsigned int y) {
	POINT p;
	p.x = x;
	p.y = y;

	ClientToScreen(window->GetHWND(), &p);

	mouseX = p.x;
	mouseY = p.y;

	SetCursorPos(mouseX, mouseY);
}
