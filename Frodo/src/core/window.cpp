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

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.OutputWindow = hwnd;
	scd.SampleDesc.Count = 1;
	scd.Windowed = true;
	#ifdef _DEBUG
	if (FAILED(D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_DEBUG, 0, 0, D3D11_SDK_VERSION, &scd, &swapChain, &device, 0, &context))) {
		#else
	if (FAILED(D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, 0, 0, D3D11_SDK_VERSION, &scd, &swapChain, &device, 0, &context))) {
		#endif
		FD_FATAL("Direct3D Error: Failed to create device(ID3D11Device) and context(ID3D11DeviceContext)");
		return;
	}

	ID3D11Texture2D* tmp;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&tmp);

	device->CreateRenderTargetView(tmp, 0, &renderTarget);

	tmp->Release();

	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));
	td.ArraySize = 1;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.Format = DXGI_FORMAT_D32_FLOAT;
	td.Width = width;
	td.Height = height;
	td.SampleDesc.Count = 1;
	td.Usage = D3D11_USAGE_DEFAULT;

	device->CreateTexture2D(&td, 0, &tmp);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsd;
	ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	dsd.Format = DXGI_FORMAT_D32_FLOAT;
	dsd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	device->CreateDepthStencilView(tmp, &dsd, &depthStencilView);

	tmp->Release();

	if (depthStencilView == nullptr) {
		FD_FATAL("Direct3D Error: Failed to create depth buffer(ID3D11DepthStencilView)");
	}

	context->OMSetRenderTargets(1, &renderTarget, depthStencilView);

	isOpen = true;
	isVisible = false;

	window_handels.Add(this, hwnd);

	clearColor[3] = 1;
}


Window::~Window() {
	FD_DEBUG("Closing window");
	depthStencilView->Release();
	renderTarget->Release();
	context->Release();
	device->Release();
	swapChain->Release();
}

void Window::SwapBuffers() {

	MSG msg;
	if (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	swapChain->Present(0, 0);
}

void Window::Clear() {
	context->ClearRenderTargetView(renderTarget, clearColor);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Window::SetVisible(bool visible) {
	isVisible = visible;
	if (isVisible)
		ShowWindow(hwnd, SW_SHOW);
	else
		ShowWindow(hwnd, SW_HIDE);
	
	FD_DEBUG("Window(%d) visibility set to %s", (int)hwnd, isVisible ? "TRUE" : "FALSE");
}