#include "renderer2d.h"
#include <core/log.h>

Renderer2D::Renderer2D(unsigned int maxRenderables) {
	this->maxRenderables = maxRenderables;

	D3D11_BUFFER_DESC d;
	ZeroMemory(&d, sizeof(D3D11_BUFFER_DESC));

	d.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d.ByteWidth = sizeof(VertexData) * maxRenderables * 4; 
	d.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d.StructureByteStride = sizeof(VertexData);
	d.Usage = D3D11_USAGE_DYNAMIC;

	D3DContext::GetDevice()->CreateBuffer(&d, 0, &buffer);

	FD_ASSERT(buffer);

	unsigned int numIndices = maxRenderables * 6;
	unsigned int* indices = new unsigned int[numIndices];

	for (unsigned int i = 0; i < maxRenderables; i++) {
		indices[i * 6 + 0] = (i * 4) + 0;
		indices[i * 6 + 1] = (i * 4) + 1;
		indices[i * 6 + 2] = (i * 4) + 2;
		indices[i * 6 + 3] = (i * 4) + 2;
		indices[i * 6 + 4] = (i * 4) + 3;
		indices[i * 6 + 5] = (i * 4) + 0;
	}

	indexBuffer = new IndexBuffer(indices, numIndices);

	delete[] indices;

	FD_ASSERT(indexBuffer);

	ZeroMemory(&mapResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	mapResource.RowPitch = d.ByteWidth;

	vBuffer = nullptr;
}

Renderer2D::~Renderer2D() {
	DX_FREE(buffer);
	
	delete indexBuffer;
	delete shader;
}

void Renderer2D::Begin() {
	numVertices = 0;

	D3DContext::GetDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE, 0, &mapResource);

	vBuffer = (VertexData*)mapResource.pData;
}

void Renderer2D::Submit(Renderable2D& renderable) {

}

void Renderer2D::End() {
	vBuffer = nullptr;
	D3DContext::GetDeviceContext()->Unmap(buffer, 0);
}

void Renderer2D::Present() {

	unsigned int stride = sizeof(VertexData);
	unsigned int offset = 0;

	D3DContext::GetDeviceContext()->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
	
	indexBuffer->Bind();
}

void Renderer2D::SetShader(Shader* shader) {
	if (shader != nullptr) {
		this->shader = shader;
		return;
	}
}