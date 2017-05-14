#include "fontrenderer.h"

#include <graphics/shader/shaderfactory.h>
#include <math/vec4.h>
#include <math/vec2.h>

#define FD_FONT_SHOW_TEXTURE 0

namespace FD {

struct Vertex {
	vec2 position;
	vec2 texCoords;
	vec4 color;
	float32 tid;
};


FontRenderer::FontRenderer(Window* window, uint32 max_glyphs) : BatchRenderer(window, max_glyphs, 16) {
	blending = true;
	depthTesting = false;

	BufferLayout layout;

	layout.Push<vec2>("POSITION");
	layout.Push<vec2>("TEXCOORDS");
	layout.Push<vec4>("COLOR");
	layout.Push<float32>("TID");



	shader = ShaderFactory::GetShader(FD_FONT_DEFAULT);

	shader->ShaderGenSetVariable("maxTextures", FD_SHADER_TYPE_PIXELSHADER, FD_FONT_MAX_SIMULTANEOUS_TEXTURES);

	shader->ShaderGenComplete();

	#if FD_FONT_SHOW_TEXTURE
	shader->SetVSConstantBuffer("view_data", &mat4::Identity());
	#else
	shader->SetVSConstantBuffer("view_data", &mat4::Orthographic(0.0f, (float32)window->GetWidth(), 0.0f, (float32)window->GetHeight(), -0.1f, 0.1f));
	#endif

	layout.CreateInputLayout(shader);


	uint32* indices = new uint32[max_glyphs * 6];

	for (uint32 i = 0; i < max_glyphs; i++) {
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


void FontRenderer::SubmitTextAlignLeft(const String& text, Font* font, vec2 position, vec4 color, vec2 scale) {
	//if (buffer == nullptr) Begin();
	float32 tid  = SubmitTexture(font->GetTexture());
	//float btid = SubmitTexture(background);

	uint_t textLength = text.length;
	float32 sizeX = (float32)font->GetSize() * scale.x;
	float32 sizeY = (float32)font->GetSize() * scale.y;

	float32 xPos = position.x;
	float32 yPos = position.y;

	float xPosOrigin = xPos;

	Font::FD_GLYPH glyph;

	for (uint_t i = 0; i < textLength; i++) {
		uint32 c = text[i];
		if (c == ' ') {
			xPos += sizeX / 2.0f;
			continue;
		} else if (c == '\n') {
			xPos = xPosOrigin;
			yPos += sizeY;
		}

		glyph = font->GetGlyph(c);

		float32 xa = xPos + ((float32)glyph.offset.x * scale.x);
		float32 ya = yPos - ((float32)glyph.offset.y * scale.y);

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

		indexCount += 6;

		xPos += (float)glyph.advance.x * scale.x;
	}
}

void FontRenderer::SubmitTextAlignRight(const String& text, Font* font, vec2 position, vec4 color, vec2 scale) {
	//if (buffer == nullptr) Begin();
	float32 tid = SubmitTexture(font->GetTexture());
	//float btid = SubmitTexture(background);

	uint_t textLength = text.length;
	float32 sizeX = (float32)font->GetSize() * scale.x;
	float32 sizeY = (float32)font->GetSize() * scale.y;

	uint_t lineLength = text.Find("\n", 0);
	uint_t lastNewLine = lineLength+1;

	if (lineLength == (uint_t)-1) {
		lineLength = (uint_t)font->GetFontMetrics(text, scale).x;
	} else {
		lineLength = (uint_t)font->GetFontMetrics(text.SubString(0, lineLength + 1), scale).x;
	}

	float32 xPos = position.x - lineLength;
	float32 yPos = position.y;

	Font::FD_GLYPH glyph;

	for (uint_t i = 0; i < textLength; i++) {
		uint32 c = text[i];
		if (c == ' ') {
			xPos += sizeX / 2.0f;
			continue;
		} else if (c == '\n') {
			lineLength = text.Find("\n", lastNewLine);

			if (lineLength == (uint_t)-1) {
				lineLength = (uint_t)font->GetFontMetrics(text.SubString(lastNewLine, textLength - lastNewLine), scale).x;
			} else {
				lineLength = (uint_t)font->GetFontMetrics(text.SubString(lastNewLine, lineLength + 1), scale).x;
			}

			lastNewLine = lineLength + 1;

			xPos = position.x - lineLength;
			yPos += sizeY;
		}

		glyph = font->GetGlyph(c);

		float32 xa = xPos + ((float32)glyph.offset.x * scale.x);
		float32 ya = yPos - ((float32)glyph.offset.y * scale.y);

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

		indexCount += 6;

		xPos += (float)glyph.advance.x * scale.x;
	}
}

void FontRenderer::SubmitTextAlignCenter(const String& text, Font* font, vec2 position, vec4 color, vec2 scale) {
	//if (buffer == nullptr) Begin();
	float32 tid = SubmitTexture(font->GetTexture());
	//float btid = SubmitTexture(background);

	uint_t textLength = text.length;
	float32 sizeX = (float32)font->GetSize() * scale.x;
	float32 sizeY = (float32)font->GetSize() * scale.y;

	uint_t lineLength = text.Find("\n", 0);
	uint_t lastNewLine = lineLength + 1;

	if (lineLength == (uint_t)-1) {
		lineLength = (uint_t)font->GetFontMetrics(text, scale).x;
	} else {
		lineLength = (uint_t)font->GetFontMetrics(text.SubString(0, lineLength + 1), scale).x;
	}

	float32 xPos = position.x - (lineLength >> 1);
	float32 yPos = position.y;

	Font::FD_GLYPH glyph;

	for (uint_t i = 0; i < textLength; i++) {
		uint32 c = text[i];
		if (c == ' ') {
			xPos += sizeX / 2.0f;
			continue;
		} else if (c == '\n') {
			lineLength = text.Find("\n", lastNewLine);

			if (lineLength == (uint_t)-1) {
				lineLength = (uint_t)font->GetFontMetrics(text.SubString(lastNewLine, textLength - lastNewLine), scale).x;
			} else {
				lineLength = (uint_t)font->GetFontMetrics(text.SubString(lastNewLine, lineLength + 1), scale).x;
			}

			lastNewLine = lineLength + 1;

			xPos = position.x - (lineLength >> 1);
			yPos += sizeY;
		}

		glyph = font->GetGlyph(c);

		float32 xa = xPos + ((float32)glyph.offset.x * scale.x);
		float32 ya = yPos - ((float32)glyph.offset.y * scale.y);

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

		indexCount += 6;

		xPos += (float)glyph.advance.x * scale.x;
	}
}
}