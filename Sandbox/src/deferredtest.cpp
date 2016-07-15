#include "deferredtest.h"


void DeferredTest::OnInit() {
	window->SetVSync(1);

	renderer = new DeferredRenderer(window->GetWidth(), window->GetHeight());

	camera = new UserCamera(vec3(0, 0, 0), vec3(0, 0, 0));

	renderer->SetCamera(camera);

	Model* model = MeshFactory::CreatePlane(10, 10);
	Material* material = new Material(vec4(1, 1, 1, 1));


	e = new Entity({0, -0.5f, 1}, { 90, 0, 0});

	e->SetModel(model);
	e->SetMaterial(material);

	renderer->AddEntity(e);
	renderer->AddLight(new DirectionalLight(vec3(1, 1, 1), vec3(0, -0.2f, 0)));
	renderer->AddLight(new PointLight(vec3(0, 0.45f, 6), vec3(1, 1, 1), vec3(1, 0, 0.010)));
	
}

void DeferredTest::OnUpdate(float delta) {
	//e->GetRotation() += (vec3(10, 0, 0) * delta);
	camera->Update(delta);
}

void DeferredTest::OnTick() {
	FD_INFO("FPS: %u", fps);
	fps = 0;
}

void DeferredTest::OnRender() {
	renderer->Render();

	fps++;
}

void DeferredTest::OnExit() {
	delete renderer;
}