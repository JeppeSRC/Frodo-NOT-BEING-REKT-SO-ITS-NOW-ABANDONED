#include <frodo.h>
#include <ctime>

FDApplication::FDApplication(const char* title, unsigned int width, unsigned int height) {
	window = new Window(title, width, height);
	capUpdate = 1;
}

FDApplication::~FDApplication() {
	OnExit();
	delete window;
}

void FDApplication::OnInit() { }

void FDApplication::OnUpdate(float delta) { }

void FDApplication::OnRender() { }

void FDApplication::OnExit() { }

void FDApplication::Run() {
	OnInit();
	Window& w = *window;
	unsigned int lastTime = clock();


	while (w.IsOpen()) {
		D3DContext::Clear();

		unsigned int now = clock();
		float delta = float(now - lastTime);
		lastTime = now;

		OnUpdate(delta);
		OnRender();

		w.SwapBuffers();
	}

}