#pragma once

#include <fd.h>
#include <graphics/font/font.h>
#include <graphics/render/renderer.h>
#include <graphics/d3dcontext.h>
#include <graphics/buffer/vertexbuffer.h>
#include <graphics/buffer/indexbuffer.h>
#include <graphics/buffer/bufferlayout.h>

#include <util/list.h>

#include <core/log.h>

class FDAPI FontRenderer : public Renderer {
private:
	friend struct Vertex;
private:
	IndexBuffer* ibo;
	VertexBuffer* vbo;

	Vertex* buffer;
	Shader* shader;

	List<Texture2D*> tids;

	unsigned int max_glyphs;
	unsigned int index_count;


	ID3D11BlendState* blendState[2];
	ID3D11DepthStencilState* depthState[2];

private:
	void SetBlendingInternal(bool enable_blending);
	void SetDepthInternal(bool enable_depthtesting);

	void CreateBlendStates();
	void CreateDepthStates();
public:
	FontRenderer(Window* window, unsigned int max_glyphs);
	~FontRenderer();

	void SubmitText(const String& text, Font* font, vec2 position);
	void Begin();
	void Render() override;
	void End();

	void Add(Entity* e) override { FD_WARNING("\"%s\" not implemented", __FUNCSIG__); }
	void Add(Light* l) override { FD_WARNING("\"%s\" not implemented", __FUNCSIG__); }

	void Remove(Entity* e) override { FD_WARNING("\"%s\" not implemented", __FUNCSIG__); }
	void Remove(Light* l) override { FD_WARNING("\"%s\" not implemented", __FUNCSIG__); }

};
