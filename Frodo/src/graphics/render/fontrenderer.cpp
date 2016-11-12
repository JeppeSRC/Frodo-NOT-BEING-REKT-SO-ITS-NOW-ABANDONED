#include "fontrenderer.h"

#include <graphics/shader/shaderfactory.h>

#define FD_FONT_MAX_SIMULTANEOUS_FONTS 4

#define FD_FONT_SHOW_TEXTURE 0

struct Vertex {
	vec2 position;
	vec2 texCoords;
	float tid;
};

void FontRenderer::SetBlendingInternal(bool enable_blending) {
	float factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	D3DContext::GetDeviceContext()->OMSetBlendState(blendState[enable_blending ? 1 : 0], factor, 0xFFFFFFFF);
}

void FontRenderer::SetDepthInternal(bool enable_depthtesting) {
	D3DContext::GetDeviceContext()->OMSetDepthStencilState(depthState[enable_depthtesting ? 1 : 0], 0);
}

void FontRenderer::CreateBlendStates() {

	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));


	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3DContext::GetDevice()->CreateBlendState(&desc, &blendState[0]);

	// ENABLED

	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3DContext::GetDevice()->CreateBlendState(&desc, &blendState[1]);
}


void FontRenderer::CreateDepthStates() {


	//DEFAULT
	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	desc.DepthEnable = false;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	desc.StencilEnable = false;

	D3DContext::GetDevice()->CreateDepthStencilState(&desc, &depthState[1]);

	ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));


	//NO DEPTH

	ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_ALWAYS;

	desc.StencilEnable = false;

	D3DContext::GetDevice()->CreateDepthStencilState(&desc, &depthState[0]);
}

FontRenderer::FontRenderer(Window* window, unsigned int max_glyphs) : Renderer(window, nullptr ){
	this->maxGlyphs = max_glyphs;
	this->indexCount = 0;
	this->buffer = nullptr;

	unsigned int* indices = new unsigned int[max_glyphs * 6];

	for (unsigned int i = 0; i < max_glyphs; i++) {
		indices[i * 6 + 0] = i * 4 + 0;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;
		indices[i * 6 + 3] = i * 4 + 2;
		indices[i * 6 + 4] = i * 4 + 3;
		indices[i * 6 + 5] = i * 4 + 0;
	}

	ibo = new IndexBuffer(indices, max_glyphs * 6);
	vbo = new VertexBuffer(sizeof(Vertex), max_glyphs * 4);

	delete[] indices;

	BufferLayout layout;

	layout.Push<vec2>("POSITION");
	layout.Push<vec2>("TEXCOORDS");
	layout.Push<float>("TID");

	shader = ShaderFactory::GetShader(FD_FONT_DEFAULT);
#if FD_FONT_SHOW_TEXTURE
	shader->SetVSConstantBuffer("view_data", &mat4::Identity());
#else
	shader->SetVSConstantBuffer("view_data", &mat4::Orthographic(0.0f, (float)window->GetWidth(), 0.0f, (float)window->GetHeight(), -0.1f, 0.1f));
#endif

	layout.CreateInputLayout(shader);

	CreateBlendStates();
	CreateDepthStates();
}

FontRenderer::~FontRenderer() {
	delete ibo;
	delete vbo;
	delete shader;

	DX_FREE(blendState[0]);
	DX_FREE(blendState[1]);
	DX_FREE(depthState[0]);
	DX_FREE(depthState[1]);
}

void FontRenderer::SubmitText(const String& text, Font* font, vec2 position) {
	//if (buffer == nullptr) Begin();
	Texture2D* tex = font->GetTexture();

	size_t numTids = tids.GetSize();

	float tid = 0;

	if (tids.Find(tex) == (size_t)-1) {
		if (tids.GetSize() == FD_FONT_MAX_SIMULTANEOUS_FONTS) {
			End();
			Render();
			Begin();

			numTids = 0;
		}

		tids.Push_back(tex);
		tid = (float)numTids + 1.0f;

	}
	else {
		tid = (float)tids.Find(tex) + 1.0f;
	}

	size_t textLength = text.length;
	float size = (float)font->GetSize();

	ivec2 dpi = window->GetMonitorDpi();

	float xPos = position.x * (float)dpi.x;
	float yPos = position.y * (float)dpi.y;

	Font::FD_GLYPH prevGlyph;

	for (size_t i = 0; i < textLength; i++) {
		unsigned int c = text[i];
		if (c == ' ') {
			xPos += size / 2.0f;
			prevGlyph.unicodeCharacter = 0;
			continue;
		}
		Font::FD_GLYPH glyph = font->GetGlyph(c);

		ivec2 kerning = font->GetKerning(prevGlyph.unicodeCharacter, glyph.unicodeCharacter);

		float xa = xPos + glyph.offset.x;
		float ya = yPos - glyph.offset.y;

#if (!FD_FONT_SHOW_TEXTURE)

		buffer->position = vec2(xa, ya);
		buffer->texCoords = vec2(glyph.u0, glyph.v0);
		buffer->tid = tid;
		buffer++;

		buffer->position = vec2(xa + size, ya);
		buffer->texCoords = vec2(glyph.u1, glyph.v0);
		buffer->tid = tid;
		buffer++;

		buffer->position = vec2(xa + size, ya + size);
		buffer->texCoords = vec2(glyph.u1, glyph.v1);
		buffer->tid = tid;
		buffer++;

		buffer->position = vec2(xa, ya + size);
		buffer->texCoords = vec2(glyph.u0, glyph.v1);
		buffer->tid = tid;
		buffer++;

#else
	

		buffer->position = vec2(-1, 1);
		buffer->texCoords = vec2(0, 0);
		buffer->tid = tid;
		buffer++;

		buffer->position = vec2(1, 1);
		buffer->texCoords = vec2(1, 0);
		buffer->tid = tid;
		buffer++;

		buffer->position = vec2(1, -1);
		buffer->texCoords = vec2(1, 1);
		buffer->tid = tid;
		buffer++;

		buffer->position = vec2(-1, -1);
		buffer->texCoords = vec2(0, 1);
		buffer->tid = tid;
		buffer++;

#endif

		indexCount += 6;

		xPos += glyph.advance.x;

		prevGlyph = glyph;
	}
}


void FontRenderer::Begin() {
	indexCount = 0;
	tids.Clear();
	buffer = (Vertex*)vbo->Map(FD_MAP_WRITE_DISCARD);
}

void FontRenderer::Render() {
//	if (buffer != nullptr) End();
	SetDepthInternal(false);
	SetBlendingInternal(true);
	for (size_t i = 0; i < tids.GetSize(); i++) {
		shader->SetTexture((unsigned int)i, tids[i]);
	}
	shader->Bind();

	vbo->Bind();
	ibo->Bind();
	
	D3DContext::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
}

void FontRenderer::End() {
	vbo->Unmap();
	buffer = nullptr;
}