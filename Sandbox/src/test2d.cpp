#include "test2d.h"
#pragma warning(disable : 4700)
Test2D::Test2D() : FDApplication("Test2D", 1280, 720) { fps = 0; }


void Test2D::OnInit() {
	Shader* shader = new Shader("res/test2dvertex.hlsl", "res/test2dpixel.hlsl");
	renderer = new Renderer2D(500000, shader);

	renderer->SetProjectionMatrix(mat4::Orthographic(-16, 16, 9, -9, -1, 1));
	
	
	for (float x = -16; x < 16; x+=0.25f) {
		for (float y = 10; y > -10; y-=0.25f) {
			unsigned int tmp = 0;
			_rdrand32_step(&tmp);
			renderables.Push_back(new Renderable2D(vec2(x, y), vec2(0.25f, 0.25f), (tmp | ((0xFF) << 24)) & 0xFFFF00FF));
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

	size_t size = renderables.GetSize();
	for (unsigned int i = 0; i < size; i++) {
		renderer->Submit(*renderables[i]);
	}

	renderer->End();
	renderer->Present();

	fps++;
}

void Test2D::OnExit() {
	delete renderer;
}

