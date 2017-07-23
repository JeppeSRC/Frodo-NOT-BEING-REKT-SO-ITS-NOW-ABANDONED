#pragma once

#include <fd.h>
#include <graphics/buffer/vertexbuffer.h>
#include <graphics/buffer/indexbuffer.h>
#include <graphics/render/material/material.h>

namespace FD {

class FDAPI Mesh {
protected:
	VertexBuffer* vBuffer;
	IndexBuffer* iBuffer;

	Material* material;

public:
	Mesh(VertexBuffer* vBuffer, IndexBuffer* iBuffer, Material* material) : vBuffer(vBuffer), iBuffer(iBuffer), material(material) {}
	~Mesh() { delete material, vBuffer, iBuffer; }

	void Render();
	void Render(Shader* shader);
	void RenderWithoutMaterial();

	inline VertexBuffer* GetVertexBuffer() { return vBuffer; }
	inline IndexBuffer* GetIndexBuffer() { return iBuffer; }
	inline Material* GetMaterial() { return material; }
};

}