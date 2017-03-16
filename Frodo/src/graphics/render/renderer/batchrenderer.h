#pragma once

#include <fd.h>
#include <graphics/font/font.h>
#include <graphics/render/renderer/renderer.h>
#include <graphics/d3dcontext.h>
#include <graphics/buffer/vertexbuffer.h>
#include <graphics/buffer/indexbuffer.h>
#include <graphics/buffer/bufferlayout.h>

#include <util/list.h>

#include <core/log.h>

#define FD_FONT_MAX_SIMULTANEOUS_TEXTURES 16

namespace FD {

class FDAPI BatchRenderer : public Renderer {
protected:
	friend struct Vertex;
protected:
	IndexBuffer* ibo;
	VertexBuffer* vbo;

	Vertex* buffer;
	Shader* shader;

	List<Texture2D*> tids;

	uint32 maxVertices;
	uint32 indexCount;


	ID3D11BlendState* blendState[2];
	ID3D11DepthStencilState* depthState[2];

	bool blending;
	bool depthTesting;

protected:
	void SetBlendingInternal(bool enable_blending);
	void SetDepthInternal(bool enable_depthtesting);

	virtual void CreateBlendStates();
	virtual void CreateDepthStates();

	float32 SubmitTexture(Texture2D* texture);

	BatchRenderer(Window* window, uint32 max_vertices);

public:
	virtual ~BatchRenderer();

	void Begin(Camera* camera) override;
	virtual void Submit(const LightStack& lights) {}
	void End() override;

	void Present() override;


};

}