#include <frodo.h>
#include <ctime>

FDApplication::FDApplication(const char* title, unsigned int width, unsigned int height) {
	window = new Window(title, width, height);
	SetUPS(60.0f);

	D3DContext::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

FDApplication::~FDApplication() {
	OnExit();
	delete window;

	VFS::Dispose();
	TextureManager::Dispose();
}

void FDApplication::OnInit() { }

void FDApplication::OnTick() { }

void FDApplication::OnUpdate(float delta) { }

void FDApplication::OnRender() { }

void FDApplication::OnExit() { }

void FDApplication::Run() {
	VFS::Init();
	TextureManager::Init();
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
			lastTime = now;
			OnUpdate(delta / 1000.0f);
			Input::Update();
		}



		OnRender();

		if (clock() - lastTime2 >= 1000) {
			lastTime2 += 1000;
			OnTick();
		}

		w.SwapBuffers();
	}

}