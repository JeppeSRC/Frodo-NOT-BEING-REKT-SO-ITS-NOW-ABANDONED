#pragma once

#include <fd.h>
#include <graphics/d3dcontext.h>
#include <graphics/buffer/indexbuffer.h>
#include <graphics/render/renderable/renderable2d.h>
#include <graphics/shader/shader.h>

struct VertexData {
	vec2 position;
	vec2 texCoords;
	unsigned int color;

};

class FDAPI Renderer2D {
private:
	ID3D11Buffer* buffer;
	D3D11_MAPPED_SUBRESOURCE mapResource;

	IndexBuffer* indexBuffer;

	Shader* shader;

	unsigned int maxRenderables;
	unsigned int numVertices;

	VertexData* vBuffer;

public:
	Renderer2D(unsigned int maxRenderables);
	~Renderer2D();


	void Begin();
	void Submit(Renderable2D& renderable);
	void End();
	void Present();

	void SetShader(Shader* shader);

	inline Shader* GetShader() const { return shader; }
	inline unsigned int GetMaxRenderables() const { return maxRenderables; }
	inline unsigned int GetNumVertices() const { return numVertices; }
};