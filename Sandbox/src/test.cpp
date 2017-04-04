#include "test.h"

using namespace FD;

float value = 0;

void Test::OnInit() {
	window->SetVSync(1);

	renderer = new FontRenderer(window, 1000);
	font = new Font("res/arial.ttf", 64, Window::GetMonitorDpi());
	Font::SetDefaultFont(font);

	menuRenderer = new MenuRenderer(window, 128);
	handler = new UIHandler;

	UIButton* button = new UIButton("Button", vec2(100, 300), vec2(250, 50));
	button->SetFont(font);

	

	UISlider* slider = new UISlider("Slider", vec2(100, 400), vec2(400, 55), new ValueInterpolation<float>(&value, 0.0f, 1.0f));
	slider->SetFont(font);


	handler->Add(slider);
	handler->Add(button);
}

void Test::OnUpdate(float delta) {
	handler->Update();
}

void Test::OnTick() {

}

void Test::OnRender() {
	renderer->Begin(nullptr);
	renderer->SubmitText("Test Text", font, vec2(10, 100), vec4(value, 0, 0, 1), vec2(1, 1), FD_TEXT_ALIGN_LEFT);
	renderer->End();
	renderer->Present();

	menuRenderer->Begin(nullptr);
	menuRenderer->Submit(handler);
	menuRenderer->End();
	menuRenderer->Present();
}

void Test::OnExit() {
	
}