#include "d3dcontext.h"
#include <core/window.h>
#include <core/log.h>

namespace FD {

D3DContext* D3DContext::pContext = nullptr;

D3DContext::D3DContext() {
	device = nullptr;
	context = nullptr;
	activeContext = nullptr;
	renderTarget = nullptr;
	depthStencilView = nullptr;
	swapChain = nullptr;
	window = nullptr;
	adapter = nullptr;
	monitor = nullptr;
	swapChain->
}

D3DContext::~D3DContext() {
	DX_FREE(depthStencilView);
	DX_FREE(renderTarget);
	DX_FREE(context);
	DX_FREE(device);
	SetFullscreen(false);
	DX_FREE(swapChain);
}

void D3DContext::SetFullscreen(bool state) {
	pContext->swapChain->SetFullscreenState(state, state ? pContext->monitor->GetOutput() : nullptr);
}

void D3DContext::CreateDevice(D3DAdapter* adapter) {

	FD_ASSERT(adapter == nullptr);

	if (pContext->device) {
		DX_FREE(pContext->device);
		delete pContext->adapter;
		pContext->adapter = nullptr;
	}

#ifndef _DEBUG
	D3D11CreateDevice(adapter->GetAdapter(), D3D_DRIVER_TYPE_UNKNOWN, 0, 0, 0, 0, D3D11_SDK_VERSION, &pContext->device, 0, &pContext->context);
#else
	D3D11CreateDevice(adapter->GetAdapter(), D3D_DRIVER_TYPE_UNKNOWN, 0, D3D11_CREATE_DEVICE_DEBUG, 0, 0, D3D11_SDK_VERSION, &pContext->device, 0, &pContext->context);
#endif

	FD_ASSERT(pContext->device == nullptr);

	FD_DEBUG("[D3DContext] Device created from adapter \"%s\"", *adapter->GetName());

	pContext->activeContext = pContext->context;
	pContext->adapter = adapter;
}

void D3DContext::CreateSwapChain(Window* window, D3DOutput* output) {

	DXGI_SWAP_CHAIN_DESC scd = { 0 };
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));


	scd.BufferCount = 1;
	scd.BufferDesc = output->GetCurrentMode();
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.Flags = 0;
	scd.OutputWindow = window->GetHWND();
	scd.SampleDesc.Count = 1;
	scd.Windowed = true;

	if (pContext->swapChain) {
		SetFullscreen(false);
		DX_FREE(pContext->swapChain);
	}

	D3DFactory::GetFactory()->CreateSwapChain(pContext->device, &scd, &pContext->swapChain);

	ID3D11Texture2D* tmp = nullptr;
	pContext->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&tmp);

	pContext->device->CreateRenderTargetView(tmp, 0, &pContext->renderTarget);

	DX_FREE(tmp);

	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));

	td.ArraySize = 1;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.Format = DXGI_FORMAT_D32_FLOAT;
	td.Width = window->GetWidth();
	td.Height = window->GetHeight();
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;

	pContext->device->CreateTexture2D(&td, 0, &tmp);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsd;
	ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	dsd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsd.Format = DXGI_FORMAT_D32_FLOAT;

	pContext->device->CreateDepthStencilView(tmp, &dsd, &pContext->depthStencilView);

	DX_FREE(tmp)

	SetRenderTargets(1, &pContext->renderTarget, pContext->depthStencilView);

	SetViewPort(0.0f, 0.0f, (float32)window->GetWidth(), (float32)window->GetHeight());

	pContext->window = window;
	pContext->monitor = output;
	pContext->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void D3DContext::CreateContext(Window* window, D3DAdapter* adapter, D3DOutput* monitor) {
	if (pContext != nullptr) {
		FD_DEBUG("[D3DContext] Deleting previous context");
		delete pContext;
	}
	FD_DEBUG("[D3DContext] Createing D3DContext");
	pContext = new D3DContext;


	CreateDevice(adapter);
	CreateSwapChain(window, monitor);
}

void D3DContext::Dispose() {
	FD_DEBUG("[D3DContext] Deleting D3DContext");
	delete pContext;
}

void D3DContext::SetMode(const DXGI_MODE_DESC mode) {
	pContext->swapChain->ResizeTarget(&mode);
}

void D3DContext::Present(uint32 syncInterval, uint32 flags) {
	GetSwapChain()->Present(syncInterval, flags);
}

float32 col[4]{ 0, 0, 0, 1 };

void D3DContext::Clear(uint16 numRenderTargets) {
	for (uint16 i = 0; i < numRenderTargets; i++)
		GetDeviceContext()->ClearRenderTargetView(GetContext()->activeRenderTargets[i], col);

	GetDeviceContext()->ClearDepthStencilView(GetContext()->activeDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DContext::SetRenderTarget(ID3D11RenderTargetView* target) {
	D3DContext& c = *GetContext();
	if (target) {
		SetRenderTargets(1, &target, c.depthStencilView);
	} else {
		SetRenderTargets(1, &c.renderTarget, c.depthStencilView);

		c.SetViewPort(0, 0, (float32)c.window->GetWidth(), (float32)c.window->GetHeight());
	}
}

void D3DContext::SetRenderTargets(uint16 numRenderTargets, ID3D11RenderTargetView** target, ID3D11DepthStencilView* depthView) {
	GetDeviceContext()->OMSetRenderTargets(numRenderTargets, target, depthView);
	for (uint16 i = 0; i < numRenderTargets; i++)
		GetContext()->activeRenderTargets[i] = target[i];

	GetContext()->activeDepthStencilView = depthView;
}

void D3DContext::SetRenderTargets(uint16 numRenderTargets, ID3D11RenderTargetView** target) {
	SetRenderTargets(numRenderTargets, target, GetContext()->depthStencilView);
}


void D3DContext::SetViewPort(float32 topLeftX, float32 topLeftY, float32 width, float32 height) {
	D3D11_VIEWPORT v;
	v.TopLeftX = topLeftX;
	v.TopLeftY = topLeftY;
	v.Width = width;
	v.Height = height;
	v.MinDepth = 0.0f;
	v.MaxDepth = 1.0f;


	GetDeviceContext()->RSSetViewports(1, &v);
}

void D3DContext::SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology) {
	GetDeviceContext()->IASetPrimitiveTopology(topology);
}

}