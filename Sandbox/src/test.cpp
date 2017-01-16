#include "test.h"


void Test::OnInit() {
	window->SetVSync(1);

	renderer = new FontRenderer(window, 1000);
	font = new Font("res/arial.ttf", 64, Window::GetMonitorDpi());
}

void Test::OnUpdate(float delta) {

}

void Test::OnTick() {

}

void Test::OnRender() {
	renderer->Begin(nullptr);
	renderer->SubmitText("Test Text", font, vec2(100, 100));
	renderer->End();
	renderer->Present();
}

void Test::OnExit() {
	
}