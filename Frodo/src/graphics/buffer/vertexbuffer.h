#pragma once
#include "buffer.h"

class FDAPI VertexBuffer : public Buffer {
private:
	unsigned int stride;
	
public:
	VertexBuffer(void* data, size_t size, unsigned int stride);

	void Bind() override;
};
