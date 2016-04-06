#pragma once
#include <fd.h>
#include <util/string.h>
#include <util/map.h>

#include <graphics/d3dcontext.h>

class FDAPI Window {
private:
	static Map<HWND, Window*> window_handels;

	static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

private:
	int width, height;
	bool isOpen;
	bool isVisible;
	unsigned int vSync;
	String title;

	HWND hwnd;

	float clearColor[4];

public:
	Window(const String& title, int width, int height);
	~Window();

	void SwapBuffers();
	void Clear();

	void SetVisible(bool visible);


	inline void SetClearColor(float r, float g, float b) { clearColor[0] = r;clearColor[1] = g;clearColor[2] = b;}
	inline bool IsOpen() const { return isOpen; }
	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
	inline float GetAspectRation() const { return float(width) / height; }
	inline const String& GetTitle() const { return title; }
	inline bool IsVisible() const { return isVisible; }
	inline HWND GetHWND() const { return hwnd; }
	inline void SetVSync(unsigned int status) { vSync = status; }
};
