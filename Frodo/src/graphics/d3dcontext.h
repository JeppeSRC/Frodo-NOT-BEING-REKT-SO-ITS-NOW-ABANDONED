#pragma once
#include <fd.h>

#include <d3d11.h>
#include <d3d10.h>
#include <dxgi.h>

#include <Windows.h>

class FDAPI D3DContext {
private:
	friend class Window;
private:
	static D3DContext* pContext;

private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ID3D11RenderTargetView* renderTarget;
	ID3D11DepthStencilView* depthStencilView;
	IDXGISwapChain* swapChain;

	D3DContext();
	~D3DContext();
public:

	static void CreateContext(Window* window);
	static void Dispose();

	static void Present();
	static void Clear();

	static void SetRenderTargets(ID3D11RenderTargetView* target, ID3D11DepthStencilView* depthView);

	inline static D3DContext* GetD3DContext() { return pContext; }
	inline static ID3D11Device* GetDevice() {return pContext->device;}
	inline static ID3D11DeviceContext* GetContext() { return pContext->context; }
	inline static IDXGISwapChain* GetSwapChain() { return  pContext->swapChain; }


};
