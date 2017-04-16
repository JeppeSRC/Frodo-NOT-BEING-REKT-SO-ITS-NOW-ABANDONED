#pragma once

#include <fd.h>
#include <entity/entity.h>
#include <graphics/render/renderer/renderer.h>

namespace FD {


class FDAPI Scene {
protected:
	Window* window;
	Renderer* renderer;
	Camera* camera;


	List<Entity3D*> entities;
	List<Light*> lights;

public:
	Scene(Window* window);
	virtual ~Scene();

	virtual void Add(Entity3D* entity);
	virtual void Add(Light* light);

	virtual void Update(float delta);
	virtual void Render();

	inline void SetCamera(Camera* camera) { this->camera = camera; }

	inline Camera* GetCamera() const { return camera; }
	inline Renderer* GetRenderer() const { return renderer; }
};
}
