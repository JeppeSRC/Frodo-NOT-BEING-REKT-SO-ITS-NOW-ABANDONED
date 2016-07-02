#include "deferredtest.h"


void DeferredTest::OnInit() {
	window->SetVSync(1);

 
	renderer = new DeferredRenderer(window->GetWidth(), window->GetHeight());

	Model* model = MeshFactory::CreatePlane(1, 1);
	Material* material = new Material(vec4(1, 1, 1, 1));


	Entity* e = new Entity({0, 0, -1}, { 0, 0, 45});

	e->SetModel(model);
	e->SetMaterial(material);

	renderer->AddEntity(e);
}

void DeferredTest::OnUpdate(float delta) {
  
}

void DeferredTest::OnTick() {

}

void DeferredTest::OnRender() {
	renderer->Render();
}

void DeferredTest::OnExit() {
	delete renderer;
}