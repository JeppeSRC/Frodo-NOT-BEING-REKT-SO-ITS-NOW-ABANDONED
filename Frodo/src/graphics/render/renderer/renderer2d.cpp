#include "renderer2d.h"
#include <core/log.h>

Renderer2D::Renderer2D(unsigned int maxRenderables, Shader* shader) {
	FD_DEBUG("Creating 2D renderer! Max renderables: %u", maxRenderables);
	this->maxRenderables = maxRenderables;

	inputLayout.Push<vec2>("POSITION");
	inputLayout.Push<vec2>("TEXCOORDS");
	inputLayout.Push<unsigned int>("COLOR");

	SetShader(shader);

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
	indicesToRender = 0;

	D3DContext::GetDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);

	vBuffer = (VertexData*)mapResource.pData;
}

void Renderer2D::Submit(Renderable2D& renderable) {

	const unsigned int color = renderable.GetColor();
	const vec2& position = renderable.GetPosition();
	vec2 size = renderable.GetSize() / 2.0f;
	const float rotation = renderable.GetRotation();

	vBuffer->position = vec2(-size.GetX(), size.GetY()) + position;
	vBuffer->color = color;							  
	vBuffer->texCoords = vec2(0, 0);				  
	vBuffer++;										  
													  
	vBuffer->position = size + position;
	vBuffer->color = color;							
	vBuffer->texCoords = vec2(1, 0);				
	vBuffer++;										
													
	vBuffer->position = vec2(size.GetX(), -size.GetY()) + position;
	vBuffer->color = color;							
	vBuffer->texCoords = vec2(1, 1);				
	vBuffer++;										
													
	vBuffer->position = -size + position;
	vBuffer->color = color;
	vBuffer->texCoords = vec2(0, 1);
	vBuffer++;

	indicesToRender += 6;
}

void Renderer2D::End() {
	vBuffer = nullptr;
	D3DContext::GetDeviceContext()->Unmap(buffer, 0);
}

void Renderer2D::Present() {
	D3DContext::SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	shader->Bind();


	unsigned int stride = sizeof(VertexData);
	unsigned int offset = 0;

	D3DContext::GetDeviceContext()->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
	
	indexBuffer->Bind();

	D3DContext::GetDeviceContext()->DrawIndexed(indicesToRender, 0, 0);
}

void Renderer2D::SetShader(Shader* shader) {
	if (shader != nullptr) {
		this->shader = shader;
		inputLayout.CreateInputLayout(shader);
		return;
	}
}

void Renderer2D::SetProjectionMatrix(const mat4 matrix) {
	shader->SetVSConstantBuffer("projection_buffer", (void*)matrix.GetData());
}