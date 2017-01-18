#pragma once
#include <math/vec2.h>

#include <fd.h>
#include <util/string.h>
#include <util/map.h>
#include <core/event/eventlistener.h>
#include "log.h"

#include <graphics/d3dcontext.h>


class FDAPI Window : public EventListener {
private:
	static Map<HWND, Window*> window_handels;

	static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

private:
	int32 width, height;
	bool isOpen;
	bool isVisible;
	uint32 vSync;
	String title;

	HWND hwnd;

	float32 clearColor[4];

public:
	Window(const String& title, int32 width, int32 height);
	~Window();

	void SwapBuffers();
	void Clear();

	void SetVisible(bool visible);
	ivec2 GetWindowDpi();

	bool OnWindowActionResize(ivec2 size) override;

	inline void SetClearColor(float32 r, float32 g, float32 b) { clearColor[0] = r;clearColor[1] = g;clearColor[2] = b;}
	inline bool IsOpen() const { return isOpen; }
	inline int32 GetWidth() const { return width; }
	inline int32 GetHeight() const { return height; }
	inline float32 GetAspectRatio() const { return float32(width) / height; }
	inline const String& GetTitle() const { return title; }
	inline bool IsVisible() const { return isVisible; }
	inline HWND GetHWND() const { return hwnd; }
	inline void SetVSync(uint32 status) { vSync = status; FD_DEBUG("Vsync set to: %u", vSync); }

public:
	static ivec2 GetMonitorDpi();
};
