#include "spriterenderer.h"


namespace FD {

struct Vertex {
	vec3 position;
	vec2 texCoords;
	vec4 color;
	float32 tid;
};

SpriteRenderer::SpriteRenderer(Window* window, uint32 max_sprites, uint32 max_simultaneous_textures) : BatchRenderer(window, max_sprites * 4, max_simultaneous_textures) {
	blending = false;
	depthTesting = false;

	BufferLayout l;
	l.Push<vec3>("POSITION");
	l.Push<vec2>("TEXCOORDS");
	l.Push<vec4>("COLOR");
	l.Push<float32>("TID");



	uint32* indices = new uint32[max_sprites * 6];

	for (uint32 i = 0; i < max_sprites; i++) {
		indices[i * 6 + 0] = i * 4 + 0;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;
		indices[i * 6 + 3] = i * 4 + 2;
		indices[i * 6 + 4] = i * 4 + 3;
		indices[i * 6 + 5] = i * 4 + 0;
	}

	ibo = new IndexBuffer(indices, max_sprites * 6);
	vbo = new VertexBuffer(sizeof(Vertex), max_sprites * 4);

	delete[] indices;

	CreateDepthStates();
	CreateBlendStates();
}

SpriteRenderer::~SpriteRenderer() {

}

void SpriteRenderer::Submit(const Sprite* sprite) {
	
	float32 tid = SubmitTexture(sprite->GetTexture());

	const vec3& position = sprite->GetPosition();
	const vec2& size = sprite->GetSize();
	const vec4& color = sprite->GetColor();


	buffer->position = position;
	buffer->texCoords = vec2(0, 0);
	buffer->color = color;
	buffer->tid = tid;
	buffer++;

	buffer->position = position + vec3(size.x, 0, 0);
	buffer->texCoords = vec2(1, 0);
	buffer->color = color;
	buffer->tid = tid;
	buffer++;

	buffer->position = position + vec3(size, 0);
	buffer->texCoords = vec2(1, 1);
	buffer->color = color;
	buffer->tid = tid;
	buffer++;

	buffer->position = position + vec3(0, size.y, 0);
	buffer->texCoords = vec2(0, 1);
	buffer->color = color;
	buffer->tid = tid;
	buffer++;

	indexCount += 6;
}

}