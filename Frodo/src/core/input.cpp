#include "input.h"
#include <memory>

InputManager::InputManager(HWND window) {
	hwnd = window;
	memset(prevKeys, 0, FD_INPUT_MAX_KEYS);
	memset(keys, 0, FD_INPUT_MAX_KEYS);

	mouseCaptured = false;
	
	POINT p;

	GetCursorPos(&p);

	ScreenToClient(hwnd, &p);

	mouseX = p.x;
	mouseY = p.y;
}

bool InputManager::IsKeyDown(unsigned char key) {
	return keys[key];
}

bool InputManager::IsKeyDownOnce(unsigned char key) {
	if (keys[key] && !prevKeys[key]) {
		prevKeys[key] = true;
		return true;
	}

	return true;
}