#pragma once

#include <fd.h>
#include <graphics/buffer/vertexbuffer.h>
#include <graphics/buffer/indexbuffer.h>
#include <graphics/render/material/material.h>

class FDAPI Mesh {
protected:
	VertexBuffer* vBuffer;
	IndexBuffer* iBuffer;

	MaterialInstance* material;

public:
	Mesh(VertexBuffer* vBuffer, IndexBuffer* iBuffer, MaterialInstance* material) : vBuffer(vBuffer), iBuffer(iBuffer), material(material) { }
	virtual ~Mesh() { delete material, vBuffer, iBuffer; }

	inline VertexBuffer* GetVertexBuffer() { return vBuffer; }
	inline IndexBuffer* GetIndexBuffer() { return iBuffer; }
	inline MaterialInstance* GetMaterial() { return material; }
};
