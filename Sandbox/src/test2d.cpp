#include "test2d.h"
#pragma warning(disable : 4700)
Test2D::Test2D() : FDApplication("Test2D", 1280, 720) { fps = 0; }


void Test2D::OnInit() {
	Shader* shader = new Shader("res/test2dvertex.hlsl", "res/test2dpixel.hlsl");
	renderer = new Renderer2D(100000, shader);

	renderer->SetProjectionMatrix(mat4::Orthographic(-16, 16, 9, -9, -1, 1));
	
	
	for (float x = -16; x < 16; x+=0.5f) {
		for (float y = 10; y > -10; y-=0.5f) {
			unsigned int tmp = 0;
			_rdrand32_step(&tmp);
			renderables.Push_back(new Renderable2D(vec2(x, y), vec2(0.5f, 0.5f), tmp | ((0xFF) << 24)));
		}
	}

	FD_INFO("Rendering: %u", renderables.GetSize());

}

void Test2D::OnTick() {
	FD_INFO("FPS: %u", fps);
	fps = 0;
}

void Test2D::OnUpdate(float delta) {

}

void Test2D::OnRender() {
	renderer->Begin();

	
	for (unsigned int i = 0; i < renderables.GetSize(); i++) {
		renderer->Submit(*renderables[i]);
	}

	renderer->End();
	renderer->Present();

	fps++;
}

void Test2D::OnExit() {
	delete renderer;
}

