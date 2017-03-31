#include "menurenderer.h"
#include <graphics/shader/shaderfactory.h>

namespace FD {

struct Vertex {
	vec2 position;
	vec2 texCoord;
	vec4 color;
	float tid;
};

MenuRenderer::MenuRenderer(Window* window, unsigned int max_items) : BatchRenderer(window, max_items * 4, 16) {
	blending = true;
	depthTesting = false;

	unsigned int* indices = new unsigned int[max_items * 6];

	for (unsigned int i = 0; i < max_items; i++) {
		indices[i * 6 + 0] = i * 4 + 0;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;
		indices[i * 6 + 3] = i * 4 + 2;
		indices[i * 6 + 4] = i * 4 + 3;
		indices[i * 6 + 5] = i * 4 + 0;
	}

	ibo = new IndexBuffer(indices, max_items * 6);
	vbo = new VertexBuffer(sizeof(Vertex), maxVertices);

	delete[] indices;

	shader = ShaderFactory::GetShader(FD_UI_DEFAULT);
	shader->ShaderGenComplete();

	shader->SetVSConstantBuffer("view_data", &mat4::Orthographic(0.0f, (float)window->GetWidth(), 0.0f, (float)window->GetHeight(), -1.0f, 1.0f));

	BufferLayout layout;

	layout.Push<vec2>("POSITION");
	layout.Push<vec2>("TEXCOORDS");
	layout.Push<vec4>("COLOR");
	layout.Push<float>("TID");

	layout.CreateInputLayout(shader);

	CreateDepthStates();
	CreateBlendStates();

	fontRenderer = new FontRenderer(window, 1024);
}

MenuRenderer::~MenuRenderer() {
	delete fontRenderer;
}

void MenuRenderer::Begin(Camera* camera) {
	BatchRenderer::Begin(nullptr);
	fontRenderer->Begin(nullptr);
}

void MenuRenderer::Submit(const UIHandler* handler) {
	size_t items = handler->GetSize();

	for (size_t i = 0; i < items; i++) {
		UIItem* item = (*handler)[i];

		if (!item->IsVisible()) continue;

		const vec4& color = item->GetColor();
		const vec2& position = item->GetAbsolutePosition();
		const vec2& size = item->GetSize();
		const vec2& margin = item->GetTitleMargin();
		Texture2D* texture = item->GetTexture();
		const String& title = item->GetTitle();

		if (title.length == 0) goto skip_title;

		Font* font = Font::GetDefaultFont();

		if (item->AutoTextAdjustment()) {

			vec2 titleSize = font->GetFontMetrics(title);

			titleSize += margin;

			vec2 difference = size / titleSize;

			float finalScale = MIN(difference.x, difference.y);

			vec2 scale(finalScale, finalScale);

			titleSize += margin * 0.5f;
			titleSize *= scale;

			fontRenderer->SubmitText(title, font, position + (size / 2.0f) - (titleSize / 2.0f) + item->GetTitleOffset() * scale, item->GetTitleColor(), scale);
		} else {
			fontRenderer->SubmitText(title, font, position + item->GetTitleOffset(), item->GetTitleColor(), item->GetTtitleScale());
		}

	skip_title:

		float tid = SubmitTexture(texture);

		buffer->position = position;
		buffer->texCoord = vec2(0, 0);
		buffer->color = color;
		buffer->tid = tid;
		buffer++;

		buffer->position = vec2(position.x + size.x, position.y);
		buffer->texCoord = vec2(1, 0);
		buffer->color = color;
		buffer->tid = tid;
		buffer++;

		buffer->position = vec2(position.x + size.x, position.y + size.y);
		buffer->texCoord = vec2(1, 1);
		buffer->color = color;
		buffer->tid = tid;
		buffer++;

		buffer->position = vec2(position.x, position.y + size.y);
		buffer->texCoord = vec2(0, 1);
		buffer->color = color;
		buffer->tid = tid;
		buffer++;

		indexCount += 6;
	}
}

void MenuRenderer::End() {
	BatchRenderer::End();
	fontRenderer->End();
}

void MenuRenderer::Present() {
	BatchRenderer::Present();
	fontRenderer->Present();
}

}