#include "d3dadapter.h"
#include "d3doutput.h"
#include <core/log.h>

namespace FD {

D3DAdapter::D3DAdapter(IDXGIAdapter* adapter) {
	FD_ASSERT(adapter == nullptr);

	DXGI_ADAPTER_DESC desc;

	adapter->GetDesc(&desc);

	this->adapter = adapter;

	name = String(desc.Description);

	vendorId = desc.VendorId;
	deviceId = desc.DeviceId;
	subSystemId = desc.SubSysId;
	revision = desc.Revision;

	vram = desc.DedicatedVideoMemory;
	sram = desc.DedicatedSystemMemory;
	shared = desc.SharedSystemMemory;

}

D3DAdapter::~D3DAdapter() {
	adapter->Release();
}

List<D3DOutput*> D3DAdapter::GetOutputs() const {
	FD_ASSERT(adapter == nullptr);
	List<D3DOutput*> outputs;

	uint32 index = 0;
	IDXGIOutput* output;

	while (adapter->EnumOutputs(index++, &output) != DXGI_ERROR_NOT_FOUND) {
		outputs.Push_back(new D3DOutput(output));
	}
	
	return outputs;
}

D3DOutput* D3DAdapter::GetFirstOutput() const {
	FD_ASSERT(adapter == nullptr);

	IDXGIOutput* output = nullptr;

	FD_ASSERT(adapter->EnumOutputs(0, &output) == DXGI_ERROR_NOT_FOUND);

	return new D3DOutput(output);
}

}