#include "rTest.h"

using namespace FD;

void RTest::OnCreateWindow() {
	FD_WINDOW_PROPERTIES prop;

	prop.width = 1280;
	prop.height = 720;

	window = new Window("LeL", prop, nullptr, nullptr);
	window->SetVSync(0);
}

void RTest::OnInit() {
	camera = new UserCamera(vec3(0, 0, -4), vec3(0, 0, 0));
	camera->SetProjection(mat4::Perspective(70.0f, 16.0f / 9.0f, 0.001f, 1000.0f));
	renderer = new SimpleRenderer(window);

	Material mat(renderer->GetGeometryShader());

	cube = new Entity3D(vec3(0, 0, 0), vec3(0, 45, 0));
	cube->SetMesh(MeshFactory::CreateCube(1.5f, 1.5f, 1.5f, new MaterialInstance(mat)));

	floor = new Entity3D(vec3(0, -1.5f, 0), vec3(90, 0, 0));
	floor->SetMesh(MeshFactory::CreatePlane(100, 100, new MaterialInstance(mat)));

	renderer->Submit(new PointLight(vec3(0, 0, -3), vec3(1, 1, 1), vec3(0.5f, 0.8f, 0)));
	renderer->Submit(new PointLight(vec3(0, 0, 3), vec3(1, 1, 1), vec3(0.5f, 0.8f, 0)));
	renderer->Submit(new PointLight(vec3(0, 3, 0), vec3(1, 1, 1), vec3(0.5f, 0.8f, 0)));
	renderer->Submit(new PointLight(vec3(3, 0, 0), vec3(1, 1, 1), vec3(0.5f, 0.8f, 0)));
	renderer->Submit(new PointLight(vec3(-3, 0, 0), vec3(1, 1, 1), vec3(0.5f, 0.8f, 0)));
}

void RTest::OnTick() {
	FD_INFO("FPS: %u", fps);
	fps = 0;
}

void RTest::OnUpdate(float delta) {
	camera->Update(delta);
}

void RTest::OnRender() {
	renderer->Begin(camera);
	renderer->Submit(cube);
	renderer->Submit(floor);
	renderer->End();
	renderer->Present();

	fps++;
}