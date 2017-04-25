#pragma once

#include <fd.h>
#include <d3d11.h>
#include <d3d10.h>
#include <dxgi.h>
#include <util/list.h>
#include <util/string.h>

namespace FD {

struct FD_D3DADAPTER_INFO {
	
};

class FDAPI D3DAdapter {
private:
	friend class D3DOutput;
private:
	IDXGIAdapter* adapter;

	String name;

	uint32 vendorId;
	uint32 deviceId;
	uint32 subSystemId;
	uint32 revision;

	uint_t vram;
	uint_t sram;
	uint_t shared;
public:
	D3DAdapter(IDXGIAdapter* adapter);
	~D3DAdapter();

	List<D3DOutput*> GetOutputs() const;
	D3DOutput* GetFirstOutput() const;

	inline IDXGIAdapter* GetAdapter() const { return adapter; }

	inline const String& GetName() const { return name; }
	inline uint32 GetVendorID() const { return vendorId; }
	inline uint32 GetDeviceID() const { return deviceId; }
	inline uint32 GetSubSystemID() const { return subSystemId; }
	inline uint32 GetRevision() const { return revision; }

	inline uint_t GetVideoMemory() const { return vram; }
	inline uint_t GetSystemMemory() const { return sram; }
	inline uint_t GetSharedSystemMemory() const { return shared; }
};

}