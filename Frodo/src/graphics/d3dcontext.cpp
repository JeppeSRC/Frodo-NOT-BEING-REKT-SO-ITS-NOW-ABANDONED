#include "d3dcontext.h"
#include <core/window.h>
#include <core/log.h>

D3DContext* D3DContext::pContext = nullptr;

D3DContext::D3DContext() { }

D3DContext::~D3DContext() {
	depthStencilView->Release();
	renderTarget->Release();
	context->Release();
	device->Release();
	swapChain->Release();
}

void D3DContext::CreateContext(Window* window) {
	FD_DEBUG("Createing D3DContext");
	D3DContext* c = new D3DContext;

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Width = window->GetWidth();
	scd.BufferDesc.Height = window->GetHeight();
	scd.BufferDesc.RefreshRate.Denominator = 60;
	scd.BufferDesc.RefreshRate.Numerator = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.OutputWindow = window->GetHWND();
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = true;

#ifdef _DEBUG
	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_DEBUG, 0, 0, D3D11_SDK_VERSION, &scd, &c->swapChain, &c->device, 0, &c->context);
#else
	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, 0, 0, 0, D3D11_SDK_VERSION, &scd, &c->swapChain, &c->device, 0, &c->context);
#endif

	ID3D11Texture2D* tmp = nullptr;
	c->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&tmp);

	c->device->CreateRenderTargetView(tmp, 0, &c->renderTarget);

	tmp->Release();
	tmp = nullptr;

	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));

	td.ArraySize = 1;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.Format = DXGI_FORMAT_D32_FLOAT;
	td.Width = window->GetWidth();
	td.Height = window->GetHeight();
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;

	c->device->CreateTexture2D(&td, 0, &tmp);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsd;
	ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	dsd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsd.Format = DXGI_FORMAT_D32_FLOAT;

	c->device->CreateDepthStencilView(tmp, &dsd, &c->depthStencilView);
	
	tmp->Release();
	tmp = nullptr;

	SetRenderTargets(c->renderTarget, c->depthStencilView);

	pContext = c;
}

void D3DContext::Dispose() {
	FD_DEBUG("Deleting D3DContext");
	delete pContext;
}

void D3DContext::Present() {
	GetSwapChain()->Present(0, 0);
}

float col[4]{0, 0, 0, 1};

void D3DContext::Clear() {
	GetContext()->ClearRenderTargetView(GetD3DContext()->renderTarget, col);
	GetContext()->ClearDepthStencilView(GetD3DContext()->depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
}

void D3DContext::SetRenderTargets(ID3D11RenderTargetView* target, ID3D11DepthStencilView* depthView) {
	GetContext()->OMSetRenderTargets(1, &target, depthView);
}