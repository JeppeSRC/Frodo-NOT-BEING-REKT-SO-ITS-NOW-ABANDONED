#include "deferredtest.h"


void DeferredTest::OnInit() {
	window->SetVSync(1);

	vec4 res = mat4::Rotate(vec3(90.0f, 0, 0)) * vec4(0, 0, 1, 0);
 
	renderer = new DeferredRenderer(window->GetWidth(), window->GetHeight());

	Model* model = MeshFactory::CreatePlane(1, 1);
	Material* material = new Material(vec4(1, 1, 1, 1));


	e = new Entity({0, 0, -1}, { 0, 0, 0});

	e->SetModel(model);
	e->SetMaterial(material);

	renderer->AddEntity(e);
	renderer->AddLight(new DirectionalLight(vec3(1, 1, 1), vec3(0, -1, 0)));
	renderer->AddLight(new DirectionalLight(vec3(1, 1, 1), vec3(0, 0, 1)));
	
}

void DeferredTest::OnUpdate(float delta) {
	e->GetRotation() += (vec3(-10, 0, 0) * delta);
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