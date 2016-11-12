#pragma once
#include <fd.h>
#include <graphics/d3dcontext.h>

static unsigned int numBuffers = 0;

class FDAPI Buffer {
protected:
	ID3D11Buffer* buffer;
	size_t size;

	Buffer() { buffer = nullptr; }

public:
	virtual ~Buffer() { DX_FREE(buffer) }

	virtual void Bind() = 0;

	inline size_t GetSize() const { return size; }
	inline ID3D11Buffer* GetBuffer() const { return buffer; }
};
