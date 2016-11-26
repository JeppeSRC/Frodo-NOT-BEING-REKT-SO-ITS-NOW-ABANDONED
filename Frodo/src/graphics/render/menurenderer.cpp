#include "menurenderer.h"
#include <graphics/shader/shaderfactory.h>

struct Vertex {
	vec2 position;
	vec2 texCoord;
	vec4 color;
	float tid;
};

MenuRenderer::MenuRenderer(Window* window, unsigned int max_items) : BatchRenderer(window, max_items * 4, 16) {
	this->depthTesting = false;
	this->blending = true;

	unsigned int* indices = new unsigned int[maxVertices];

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

	BufferLayout layout;

	layout.Push<vec2>("POSITION");
	layout.Push<vec2>("TEXCOORDS");
	layout.Push<vec4>("COLOR");
	layout.Push<float>("TID");

	layout.CreateInputLayout(shader);

	CreateDepthStates();
	CreateBlendStates();
}

MenuRenderer::~MenuRenderer() {

}

void MenuRenderer::Submit(const Menu* menu) {

}