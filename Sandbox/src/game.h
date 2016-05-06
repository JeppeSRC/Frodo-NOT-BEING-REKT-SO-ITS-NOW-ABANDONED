#pragma once
#include <frodo.h>

class Game : public FDApplication {
protected:

	void OnInit() override;
	void OnUpdate(float delta) override;
	void OnRender() override;
	void OnExit() override;

public:

	IndexBuffer* i;
	IndexBuffer* i2;
	VertexBuffer* v;
	VertexBuffer* v2;
	Shader* shader;
	Shader* shader2;
	Texture* tex;
	Framebuffer2D* framebuffer;

	struct Mod {
		mat4 projection;
		mat4 model;
	} mod;

	float tmp;

	Game() : FDApplication("Title", 1000, 600) { tmp = 0; }



};
