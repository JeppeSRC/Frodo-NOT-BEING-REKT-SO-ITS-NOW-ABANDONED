#include <frodo.h>
#include <ctime>

namespace FD {

FDApplication::FDApplication(const char* title, uint32 width, uint32 height) {
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

void FDApplication::OnInit() {}

void FDApplication::OnTick() {}

void FDApplication::OnUpdate(float32 delta) {}

void FDApplication::OnRender() {}

void FDApplication::OnExit() {}

void FDApplication::Run() {
	VFS::Init();
	TextureManager::Init();
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
}
}