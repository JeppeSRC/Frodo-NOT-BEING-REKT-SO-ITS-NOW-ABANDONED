#include <frodo.h>
#include <ctime>

namespace FD {

Application::Application() {
	SetUPS(0);
}

Application::~Application() {
	OnExit();
	delete window;

	VFS::Dispose();
	TextureManager::Dispose();
}

//void Application::OnInit() {}

void Application::OnTick() {}

void Application::OnUpdate(float32 delta) {}

void Application::OnRender() {}

void Application::OnExit() {}

void Application::Run() {
	VFS::Init();
	D3DFactory::CreateFactory();
	OnCreateWindow();
	TextureManager::Init();
	AudioManager::Init();
	OnInit();
	Window& w = *window;
	uint32 lastTime = clock();
	uint32 lastTime2 = clock();
	uint32 lastTime3 = clock();

	float32 delta = 0;

	while (w.IsOpen()) {
		D3DContext::Clear();

		uint32 now = clock();

			
		if ((delta = float32(now - lastTime)) > ups) {
			lastTime = now;
			OnUpdate(delta / 1000.0f);
		}

		Input::UpdateInputAndDispatchEvents();

		OnRender();

		if (clock() - lastTime2 >= 1000) {
			lastTime2 += 1000;
			OnTick();
		}

		w.SwapBuffers();
	}

	OnExit();
	AudioManager::Release();
	D3DFactory::Release();
}
}