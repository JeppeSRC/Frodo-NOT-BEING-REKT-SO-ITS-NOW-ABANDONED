#pragma once
#include <math/vec2.h>
#include <fd.h>
#include <Windows.h>
#include <core/window.h>

#define FD_INPUT_MAX_KEYS 65535

class FDAPI Input {
private:
	static Window* window;

public:

	static void Init(Window* window);
	static void Update();

	static bool IsKeyDown(unsigned char key);
	static bool IsKeyDownOnce(unsigned char key);

	static void SetMousePos(unsigned int x, unsigned int y);

	inline static void CaptureMouse() { mouseCaptured = true; ShowCursor(FALSE); }
	inline static void ReleaseMouse() { mouseCaptured = false; ShowCursor(TRUE); }
	
	static inline bool IsMouseCaptured() { return mouseCaptured; }
	static inline unsigned int GetMouseX() { return mouseX; }
	static inline unsigned int GetMouseY() { return mouseY; }

public:
	static bool prevKeys[FD_INPUT_MAX_KEYS];
	static bool keys[FD_INPUT_MAX_KEYS];

	static unsigned int mouseX;
	static unsigned int mouseY;
	static bool mouseCaptured;
};