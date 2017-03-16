#pragma once
#include <fd.h>

#include <d3d11.h>
#include <d3d10.h>
#include <dxgi.h>

#include <Windows.h>

#define DX_FREE(x) if (x != nullptr) { x->Release(); x = nullptr;}

namespace FD {

class FDAPI D3DContext {
private:
	friend class Window;
private:
	static D3DContext* pContext;

private:
	ID3D11RenderTargetView* activeRenderTargets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	ID3D11DepthStencilView* activeDepthStencilView;

	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ID3D11DeviceContext* activeContext;
	ID3D11RenderTargetView* renderTarget;
	ID3D11DepthStencilView* depthStencilView;
	IDXGISwapChain* swapChain;

	Window* window;

	D3DContext();
	~D3DContext();
public:

	static void CreateContext(Window* window);
	static void Dispose();

	static void Present(uint32 syncInterval, uint32 flags);
	static void Clear(uint16 numRenderTargets = 1);

	static void SetRenderTarget(ID3D11RenderTargetView* target);
	static void SetRenderTargets(uint16 numRenderTargets, ID3D11RenderTargetView** target, ID3D11DepthStencilView* depthView);
	static void SetRenderTargets(uint16 numRenderTargets, ID3D11RenderTargetView** target);
	static void SetViewPort(float32 topLeftX, float32 topLeftY, float32 width, float32 height);
	static void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology);

	__forceinline static void SetActiveDeviceContext(ID3D11DeviceContext* context) { pContext->activeContext = context != nullptr ? context : pContext->context; }

	inline static ID3D11DepthStencilView* GetDefaultDepthStencilView() { return D3DContext::GetContext()->depthStencilView; }
	inline static ID3D11RenderTargetView* GetDefaultRenderTarget() { return D3DContext::GetContext()->renderTarget; }

	inline static Window* GetWindow() { return pContext->window; }

	__forceinline static D3DContext* GetContext() { return pContext; }
	__forceinline static ID3D11Device* GetDevice() { return pContext->device; }
	__forceinline static ID3D11DeviceContext* GetDeviceContext() { return pContext->activeContext; }
	__forceinline static IDXGISwapChain* GetSwapChain() { return  pContext->swapChain; }


};

}