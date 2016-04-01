#pragma once
#include <fd.h>
#include <graphics/d3dcontext.h>

class FDAPI FDBuffer {
protected:
	ID3D11Buffer* buffer;
	size_t size;

	FDBuffer() { buffer = nullptr; }

public:
	virtual ~FDBuffer() { buffer->Release(); }

	virtual void Bind() = 0;

	inline size_t GetSize() const { return size; }
	inline ID3D11Buffer* GetBuffer() const { return buffer; }
};
