#pragma once
#include "buffer.h"

class FDAPI FDVertexBuffer : public FDBuffer {
private:
	unsigned int stride;
	
public:
	FDVertexBuffer(void* data, size_t size, unsigned int stride);

	void Bind() override;
};
