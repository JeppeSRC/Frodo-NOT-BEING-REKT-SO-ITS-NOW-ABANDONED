#include "test.h"

using namespace FD;

float value = 0;

UIItem* a;

void Test::OnInit() {
	window->SetVSync(1);

	renderer = new FontRenderer(window, 1000);
	font = new Font("res/arial.ttf", 64, Window::GetMonitorDpi());
	Font::SetDefaultFont(font);

	menuRenderer = new MenuRenderer(window, 128);
	handler = new UIHandler;

	UIItem* button = new UIButton("Button", vec2(100, 300), vec2(350, 50), "Button");
	button->SetFont(font);
	button->GetText("title")->SetColor(vec4(1, 0, 1, 1));
	button->GetText("title")->SetOffset(vec2(0, -5));

	a = button;

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