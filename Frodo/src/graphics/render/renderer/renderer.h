#pragma once
#include <fd.h>
#include <core/window.h>
#include <graphics/render/camera/camera.h>
#include <entity/entity.h>
#include <graphics/render/light/light.h>

namespace FD {


class FDAPI Renderer : public EventListener {
protected:
	Window* window;

	Renderer(Window* window) { this->window = window; }
public:
	virtual ~Renderer() {}

	virtual void Begin(Camera* camera) = 0;
	virtual void Submit(const List<Light*>& lights) { FD_WARNING("Func: \"%s\" not implemented", __FUNCSIG__); }
	virtual void Submit(Light* light) { FD_WARNING("Func: \"%s\" not implemented", __FUNCSIG__); }
	virtual void Submit(Entity3D* e) { FD_WARNING("Func: \"%s\" not implemented", __FUNCSIG__); }
	virtual void End() { FD_WARNING("Func: \"%s\" not implemented", __FUNCSIG__); }
	virtual void Present() = 0;

	inline Window* GetWindow() const { return window; }
};

}