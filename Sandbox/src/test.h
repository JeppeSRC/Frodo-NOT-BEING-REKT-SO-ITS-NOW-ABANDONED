#pragma once
#include <frodo.h>

class Test : public FDApplication {
protected:

	void OnInit() override;
	void OnTick() override;
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

	unsigned int fps;

	struct Mod {
		mat4 projection;
		mat4 model;
	} mod;

	float tmp;

	Test() : FDApplication("Title", 1000, 600) { tmp = 0; }



};
