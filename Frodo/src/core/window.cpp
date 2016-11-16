#include "window.h"
#include "log.h"
#include "input.h"
#include <core/event/eventdispatcher.h>

Map<HWND, Window*> Window::window_handels;

LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l) {
	Window* window = window_handels.Retrieve(hwnd);
	Event* e;
	unsigned int x = 0;
	unsigned int y = 0;

	switch (msg) {
		case WM_CLOSE:
			window->isOpen = false;
			break;
		case WM_LBUTTONDOWN:
			e = new EventMouseActionButton(true, EventMouseActionButton::FD_LEFTBUTTON);
			EventDispatcher::DispatchEvent(e);
			break;
		case WM_LBUTTONUP:
			e = new EventMouseActionButton(false, EventMouseActionButton::FD_LEFTBUTTON);
			EventDispatcher::DispatchEvent(e);
			break;
		case WM_MBUTTONDOWN:
			e = new EventMouseActionButton(true, EventMouseActionButton::FD_MIDDLEBUTTON);
			EventDispatcher::DispatchEvent(e);
			break;
		case WM_MBUTTONUP:
			e = new EventMouseActionButton(false, EventMouseActionButton::FD_MIDDLEBUTTON);
			EventDispatcher::DispatchEvent(e);
			break;
		case WM_RBUTTONDOWN:
			e = new EventMouseActionButton(true, EventMouseActionButton::FD_RIGHTBUTTON);
			EventDispatcher::DispatchEvent(e);
			break;
		case WM_RBUTTONUP:
			e = new EventMouseActionButton(false, EventMouseActionButton::FD_RIGHTBUTTON);
			EventDispatcher::DispatchEvent(e);
			break;
		case WM_MOUSEMOVE:
			x = LOWORD(l);
			y = HIWORD(l);
			Input::mouseX = x;
			Input::mouseY = y;

			e = new EventMouseActionMove(ivec2(x, y));
			EventDispatcher::DispatchEvent(e);
			break;
		case WM_KEYDOWN:
			Input::keys[(unsigned char)w] = true;
			e = new EventKeyboardActionKey(true, w);
			EventDispatcher::DispatchEvent(e);
			break;
		case WM_KEYUP:
			Input::keys[(unsigned char)w] = false;
			Input::prevKeys[(unsigned char)w] = false;
			e = new EventKeyboardActionKey(false, w);
			EventDispatcher::DispatchEvent(e);
			break;
		case WM_MOVE:
			x = LOWORD(l);
			y = HIWORD(l);
			e = new EventWindowActionMove(ivec2(x, y));
			EventDispatcher::DispatchEvent(e);
			break;
		case WM_SIZING:
			x = LOWORD(l);
			y = HIWORD(l);
			e = new EventWindowActionResize(ivec2(x, y));
			EventDispatcher::DispatchEvent(e);
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

	if (!(hwnd = CreateWindow("Frodo Window", *title, WS_OVERLAPPEDWINDOW, (GetSystemMetrics(SM_CXSCREEN) >> 1) - (width >> 1), (GetSystemMetrics(SM_CYSCREEN) >> 1) - (height >> 1), r.right - r.left, r.bottom - r.top, 0, 0, 0, 0))) {
		FD_FATAL("Failed to create window (HWND)");
		return;
	}

	D3DContext::CreateContext(this);

	isOpen = true;
	SetVisible(true);
	SetVSync(0);
	Input::Init(this);

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


ivec2 Window::GetMonitorDpi() {
	HDC m = GetDC(0);

	int x = GetDeviceCaps(m, LOGPIXELSX);
	int y = GetDeviceCaps(m, LOGPIXELSY);

	ReleaseDC(0, m);

	return ivec2(x, y);
}

ivec2 Window::GetWindowDpi() {
	HDC m = GetDC(hwnd);

	int x = GetDeviceCaps(m, LOGPIXELSX);
	int y = GetDeviceCaps(m, LOGPIXELSY);

	ReleaseDC(hwnd, m);

	return ivec2(x, y);
}