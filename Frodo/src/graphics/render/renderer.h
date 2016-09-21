#pragma once
#include <fd.h>
#include <core/window.h>
#include <graphics/render/camera/camera.h>
#include <entity/entity.h>
#include <graphics/render/light/light.h>

class FDAPI Renderer {
protected:
	Window* window;
	Camera* camera;

public:
	Renderer(Window* window, Camera* camera) {
		this->window = window;
		this->camera = camera;
	}

	virtual void Render() = 0;

	virtual void Add(Entity* entity) = 0;
	virtual void Add(Light* light) = 0;

	virtual void Remove(Entity* entity) = 0;
	virtual void Remove(Light* light) = 0;

	inline Window* GetWindow() const { return window; }
	inline Camera* GetCamera() const { return camera; }

	inline void SetCamera(Camera* camera) { this->camera = camera; }
};
