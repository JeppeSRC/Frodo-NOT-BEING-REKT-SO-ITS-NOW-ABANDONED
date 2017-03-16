#pragma once
#include <fd.h>
#include <graphics/d3dcontext.h>

namespace FD {

static uint32 numBuffers = 0;

class FDAPI Buffer {
protected:
	ID3D11Buffer* buffer;
	uint_t size;

	Buffer() { buffer = nullptr; }

public:
	virtual ~Buffer() { DX_FREE(buffer) }

	virtual void Bind() = 0;

	inline uint_t GetSize() const { return size; }
	inline ID3D11Buffer* GetBuffer() const { return buffer; }
};

}