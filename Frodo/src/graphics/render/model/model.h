#pragma once

#include <fd.h>
#include <math/math.h>
#include <graphics/buffer/vertexbuffer.h>
#include <graphics/buffer/indexbuffer.h>


class FDAPI Model {
private:
	VertexBuffer* vBuffer;
	IndexBuffer*  iBuffer;

public:

	Model(){ }
	Model(VertexBuffer* v, IndexBuffer* i) : vBuffer(v), iBuffer(i) { }
	~Model() { delete vBuffer, iBuffer; }

	inline void Bind() const { vBuffer->Bind(); iBuffer->Bind(); }

	inline VertexBuffer* GetVertexBuffer() const { return vBuffer; }
	inline IndexBuffer* GetIndexBuffer() const { return iBuffer; }
};
