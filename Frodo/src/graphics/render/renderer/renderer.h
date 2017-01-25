#pragma once
#include <fd.h>
#include <core/window.h>
#include <graphics/render/camera/camera.h>
#include <entity/entity.h>
#include <graphics/render/light/light.h>

struct RenderCommand {
	Mesh* mesh;
	mat4 transform;
	Shader* shader;
};

class FDAPI Renderer : public EventListener {
protected:
	Window* window;

	List<RenderCommand> commandQueue;
	
	Renderer(Window* window) { this->window = window; }
public:
	virtual ~Renderer() {}

	virtual void Present() = 0;

	virtual void Begin(Camera* camera) = 0;
	virtual void Submit(const LightStack& lights) = 0;
	virtual void Submit(const RenderCommand& cmd);
	virtual void Submit(Entity* e);
	virtual void Submit(Mesh* mesh, mat4 transform);
	virtual void End() = 0;


	inline Window* GetWindow() const { return window; }
	inline List<RenderCommand> GetCommandQueue() const { return commandQueue; }
};
