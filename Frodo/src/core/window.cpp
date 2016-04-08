#include "window.h"
#include "log.h"

Map<HWND, Window*> Window::window_handels;

LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l) {
	Window* window = window_handels.Retrieve(hwnd);
	
	switch (msg) {
		case WM_CLOSE:
			window->isOpen = false;
			break;
	}

	return DefWindowProc(hwnd, msg, w, l);
}

Window::Window(const String& title, int width, int height) : title(title), width(width), height(height) {
	FD_DEBUG("Creating window Title<%s> Width<%d> Height<%d>!", *title, width, height);

	WNDCLASSEX ws;

	ws.cbClsExtra = 0;
	ws.cbSize = sizeof(WNDCLASSEX);
	ws.cbWndExtra = 0;
	ws.hbrBackground = 0;
	ws.hCursor = LoadCursor(0, IDC_ARROW);
	ws.hIcon = LoadIcon(0, IDI_WINLOGO);
	ws.hIconSm = 0;
	ws.hInstance = 0;
	ws.lpfnWndProc = (WNDPROC)WndProc;
	ws.lpszClassName = "Frodo Window";
	ws.lpszMenuName = 0;
	ws.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&ws)) {
		FD_FATAL("Failed to register (WNDCLASSEX)");
		return;
	}

	RECT r = {0,0,width, height};

	AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);

	if (!(hwnd = CreateWindow("Frodo Window", *title, WS_OVERLAPPEDWINDOW, GetSystemMetrics(SM_CXSCREEN) >> 1, GetSystemMetrics(SM_CYSCREEN) >> 1, r.right - r.left, r.bottom - r.top, 0, 0, 0, 0))) {
		FD_FATAL("Failed to create window (HWND)");
		return;
	}

	D3DContext::CreateContext(this);

	isOpen = true;
	isVisible = false;
	SetVSync(0);

	window_handels.Add(this, hwnd);

	clearColor[3] = 1;
}


Window::~Window() {
	FD_DEBUG("Closing window");
	D3DContext::Dispose();
}

void Window::SwapBuffers() {

	MSG msg;
	if (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	D3DContext::Present(vSync, 0);
}

void Window::Clear() {
	D3DContext::Clear();
}

void Window::SetVisible(bool visible) {
	isVisible = visible;
	if (isVisible)
		ShowWindow(hwnd, SW_SHOW);
	else
		ShowWindow(hwnd, SW_HIDE);
	
	FD_DEBUG("Window(%d) visibility set to %s", (int)hwnd, isVisible ? "TRUE" : "FALSE");
}