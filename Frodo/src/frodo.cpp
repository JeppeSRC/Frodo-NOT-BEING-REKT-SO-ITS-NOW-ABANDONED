#include <frodo.h>
#include <ctime>

FDApplication::FDApplication(const char* title, unsigned int width, unsigned int height) {
	window = new Window(title, width, height);
	SetUPS(60.0f);
}

FDApplication::~FDApplication() {
	OnExit();
	delete window;
}

void FDApplication::OnInit() { }

void FDApplication::OnTick() { }

void FDApplication::OnUpdate(float delta) { }

void FDApplication::OnRender() { }

void FDApplication::OnExit() { }

void FDApplication::Run() {
	OnInit();
	Window& w = *window;
	unsigned int lastTime = clock();
	unsigned int lastTime2 = clock();
	unsigned int lastTime3 = clock();

	float delta = 0;

	while (w.IsOpen()) {
		D3DContext::Clear();

		unsigned int now = clock();

		if ((delta = float(now - lastTime)) > ups) {
			lastTime += (unsigned int)ups;
			OnUpdate(delta);
			Input::Update();
			lastTime = now;
		}



		OnRender();

		if (clock() - lastTime2 >= 1000) {
			lastTime2 += 1000;
			OnTick();
		}

		w.SwapBuffers();
	}

}