#pragma once
#include <math/vec2.h>
#include <fd.h>
#include <Windows.h>

#define FD_INPUT_MAX_KEYS 65535

class FDAPI InputManager {
private:

	unsigned char prevKeys[FD_INPUT_MAX_KEYS];
	unsigned char keys[FD_INPUT_MAX_KEYS];
	
	bool mouseCaptured;
	unsigned int mouseX;
	unsigned int mouseY;

	HWND hwnd;

public:
	InputManager(HWND window);

	bool IsKeyDown(unsigned char key);
	bool IsKeyDownOnce(unsigned char key);


	inline bool	IsMouseCaptured() const { return mouseCaptured; }
	inline unsigned int GetMouseX() const { return mouseX; }
	inline unsigned int GetMouseY() const { return mouseY; }
};