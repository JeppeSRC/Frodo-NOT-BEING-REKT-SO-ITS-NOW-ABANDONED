#include "test.h"

using namespace FD;

float value = 0.5f;

char* string;

void Test::OnInit() {
	window->SetVSync(0);

	renderer = new FontRenderer(window, 1000);

	Font::FD_RANGE<> r;

	r.start = 0x20;
	r.end = 0x7E;

	font = new Font("res/arial.ttf", 128, Window::GetMonitorDpi(), &r, 1);
	Font::SetDefaultFont(font);
	Texture2D* mountains = new Texture2D("res/cube.png");
	menuRenderer = new MenuRenderer(window, 128);
	spriteRenderer = new SpriteRenderer(window, 500000, 64);
	handler = new UIHandler;

	UIItem* button = new UIButton("Button", vec2(100, 300), vec2(350, 50), "Button");
	button->SetFont(font);
	button->GetText("title")->SetColor(vec4(1, 0, 1, 1));
	button->GetText("title")->SetOffset(vec2(-5, -20));

	Sprite* sprite = new Sprite(vec3(400, 200, 0), vec2(100, 100), vec4(1, 0.5f, 0.25f, 1));
	UISlider* slider = new UISlider("Slider", vec2(100, 400), vec2(400, 55), new ValueInterpolation<float>((float*)&sprite->GetPosition().x, 0.0f, (float32)window->GetWidth() - sprite->GetSize().x));
	slider->SetFont(font);

	textbox = new UITextBox("TextBox", vec2(550, 100), vec2(400, 50));
	
	
	textbox->GetText<UITextAutoResize>("content")->SetMargin(vec2(20, 60));

	for (uint32 x = 0; x < window->GetWidth(); x+=3) {
		for (uint32 y = 0; y < window->GetHeight(); y+=3) {
			sprites.Push_back(new Sprite(vec3(x, y, 0), vec2(2, 2), mountains));
		}
	}

	string = new char[100];
	memset(string, 0, 100);

	sprintf(string, "Sprites: %u", sprites.GetSize());

	sprites.Push_back(sprite);

	handler->Add(textbox);
	handler->Add(slider);
	handler->Add(button);
}

void Test::OnUpdate(float delta) {
	handler->Update(delta);
}

void Test::OnTick() {

}

void Test::OnRender() {
	spriteRenderer->Begin(nullptr);
	for (uint_t i = 0; i < sprites.GetSize(); i++)
		spriteRenderer->Submit(sprites[i]);
	spriteRenderer->End();
	spriteRenderer->Present();

	renderer->Begin(nullptr);
	renderer->SubmitText(string, font, vec2(10, 100), vec4(value, 0, 0, 1), font->GetScaleFromSize(64), FD_TEXT_ALIGN_LEFT);
	renderer->End();
	renderer->Present();

	menuRenderer->Begin(nullptr);
	menuRenderer->Submit(handler);
	menuRenderer->End();
	menuRenderer->Present();
}

void Test::OnExit() {
	
}