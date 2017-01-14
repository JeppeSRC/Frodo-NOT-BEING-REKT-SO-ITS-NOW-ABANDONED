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
	BatchRenderer(Window* window, Camera* camera, uint32 max_glyphs);
	virtual ~BatchRenderer();

	void Begin();
	virtual void Submit(Entity* entity);
	void End();

	void Render();

	virtual void Add(Entity* e) override { FD_WARNING("\"%s\" not implemented", __FUNCSIG__); }
	virtual void Add(Light* l) override { FD_WARNING("\"%s\" not implemented", __FUNCSIG__); }

	virtual void Remove(Entity* e) override { FD_WARNING("\"%s\" not implemented", __FUNCSIG__); }
	virtual void Remove(Light* l) override { FD_WARNING("\"%s\" not implemented", __FUNCSIG__); }

};

