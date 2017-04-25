#pragma once

#include <fd.h>
#include <d3d11.h>
#include <d3d10.h>
#include <dxgi.h>

#include <util/list.h>

namespace FD {

class FDAPI D3DFactory {
private:
	friend class D3DAdapter;
private:
	static IDXGIFactory* factory;

public:

	static void CreateFactory();
	static void Release();
	
	static List<D3DAdapter*> GetAdapters();
	static D3DAdapter* GetFirstAdapter();

	inline static IDXGIFactory* GetFactory() { return factory; }
};

}