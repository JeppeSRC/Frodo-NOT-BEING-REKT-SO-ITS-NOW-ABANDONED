#include "fontrenderer.h"

#include <graphics/shader/shaderfactory.h>
#include <math/vec4.h>
#include <math/vec2.h>

#define FD_FONT_SHOW_TEXTURE 0

struct Vertex {
	vec2 position;
	vec2 texCoords;
	vec4 color;
	float tid;
};

FontRenderer::FontRenderer(Window* window, unsigned int max_glyphs) : BatchRenderer(window, max_glyphs, 16) { 
	blending = true; 
	depthTesting = false; 

	BufferLayout layout;

	layout.Push<vec2>("POSITION");
	layout.Push<vec2>("TEXCOORDS");
	layout.Push<vec4>("COLOR");
	layout.Push<float>("TID");

	

	shader = ShaderFactory::GetShader(FD_FONT_DEFAULT);
#if FD_FONT_SHOW_TEXTURE
	shader->SetVSConstantBuffer("view_data", &mat4::Identity());
#else
	shader->SetVSConstantBuffer("view_data", &mat4::Orthographic(0.0f, (float)window->GetWidth(), 0.0f, (float)window->GetHeight(), -0.1f, 0.1f));
#endif

	layout.CreateInputLayout(shader);

	
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

	CreateBlendStates();
	CreateDepthStates();
}

FontRenderer::~FontRenderer() {

}

void FontRenderer::SubmitText(const String& text, Font* font, vec2 position, vec4 color, vec2 scale) {
	//if (buffer == nullptr) Begin();
	float tid  = SubmitTexture(font->GetTexture());
	//float btid = SubmitTexture(background);

	size_t textLength = text.length;
	float sizeX = (float)font->GetSize() * scale.x;
	float sizeY = (float)font->GetSize() * scale.y;

	ivec2 dpi = window->GetMonitorDpi();

	float xPos = position.x;
	float yPos = position.y;

	float xPosOrigin = xPos;

	Font::FD_GLYPH glyph;

	for (size_t i = 0; i < textLength; i++) {
		unsigned int c = text[i];
		if (c == ' ') {
			xPos += sizeX / 2.0f;
			continue;
		} else if (c == '\n') {
			xPos = xPosOrigin;
			yPos += sizeY;
		}

		glyph = font->GetGlyph(c);

		float xa = xPos + ((float)glyph.offset.x * scale.x);
		float ya = yPos - ((float)glyph.offset.y * scale.y);

#if (!FD_FONT_SHOW_TEXTURE)

		buffer->position = vec2(xa, ya);
		buffer->texCoords = vec2(glyph.u0, glyph.v0);
		buffer->color = color;
		buffer->tid = tid;
		buffer++;

		buffer->position = vec2(xa + sizeX, ya);
		buffer->texCoords = vec2(glyph.u1, glyph.v0);
		buffer->color = color;
		buffer->tid = tid;
		buffer++;

		buffer->position = vec2(xa + sizeX, ya + sizeY);
		buffer->texCoords = vec2(glyph.u1, glyph.v1);
		buffer->color = color;
		buffer->tid = tid;
		buffer++;

		buffer->position = vec2(xa, ya + sizeY);
		buffer->texCoords = vec2(glyph.u0, glyph.v1);
		buffer->color = color;
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

		xPos += (float)glyph.advance.x * scale.x;
	}
}
