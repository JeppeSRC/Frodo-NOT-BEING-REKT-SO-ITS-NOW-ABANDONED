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

	static bool IsKeyDown(byte key);
	static bool IsKeyDownOnce(byte key);

	static void SetMousePos(uint32 x, uint32 y);

	inline static void CaptureMouse() { mouseCaptured = true; ShowCursor(FALSE); }
	inline static void ReleaseMouse() { mouseCaptured = false; ShowCursor(TRUE); }
	
	static inline bool IsMouseCaptured() { return mouseCaptured; }
	static inline uint32 GetMouseX() { return mouseX; }
	static inline uint32 GetMouseY() { return mouseY; }

public:
	static bool prevKeys[FD_INPUT_MAX_KEYS];
	static bool keys[FD_INPUT_MAX_KEYS];

	static uint32 mouseX;
	static uint32 mouseY;
	static bool mouseCaptured;
};