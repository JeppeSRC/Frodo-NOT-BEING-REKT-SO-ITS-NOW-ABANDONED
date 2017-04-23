#include "d3dfactory.h"
#include "d3dadapter.h"

#include <core/log.h>

namespace FD {

IDXGIFactory* D3DFactory::factory = nullptr;

void D3DFactory::CreateFactory() {
	if (CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory) != S_OK) {
		FD_ASSERT("[D3DFactory] Failed to create factory");
		return;
	}

	FD_DEBUG("[D3DFactory] D3DFactory created");
}

void D3DFactory::Release() {
	factory->Release();
	factory = nullptr;
	FD_DEBUG("[D3DFactory] D3DFactory released");
}

List<D3DAdapter*> D3DFactory::GetAdapters() {
	FD_ASSERT(factory == nullptr, "CreateFactory must be called first");

	uint32 index = 0;

	List<D3DAdapter*> adapters;
	IDXGIAdapter* adapter = nullptr;
	while (factory->EnumAdapters(index++, &adapter) != DXGI_ERROR_NOT_FOUND) {
		adapters.Push_back(new D3DAdapter(adapter));
	}

	return adapters;
}

}