#include "scene.h"
#include <graphics/pbr/render/pbrrenderer.h>

namespace FD {


Scene::Scene(Window* window) : window(window) {
	renderer = new PBRStaticRenderer(window);
}

Scene::~Scene() {

}

void Scene::Add(Entity3D* entity) {
	entities.Push_back(entity);
}

void Scene::Add(Light* light) {
	lights.Push_back(light);
}

void Scene::Update(float delta) {

}

void Scene::Render() {
	LightStack stack;
	stack.Add(lights[0]);

	renderer->Begin(camera);
	renderer->Submit(stack);

	for (uint_t i = 0; i < entities.GetSize(); i++) {
		renderer->Submit(entities[i]);
	}

	renderer->End();
	renderer->Present();
}
}