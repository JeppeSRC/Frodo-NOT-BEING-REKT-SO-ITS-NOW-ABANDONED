#pragma once

#include <fd.h>
#include <graphics/render/light/light.h>
#include <graphics/shader/shader.h>
#include <graphics/render/renderer/renderer.h>
#include <graphics/render/camera/camera.h>
#include <entity/entity.h>

namespace FD {

struct RenderCommand {
	Mesh* mesh;
	mat4 transform;
	Shader* shader;
};

class FDAPI PBRStaticRenderer : public Renderer {
private:
	Shader::ConstantBufferSlot light;
	Shader::ConstantBufferSlot camera;

	List<RenderCommand> commandQueue;
public:
	PBRStaticRenderer(Window* window);
	~PBRStaticRenderer();

	virtual void Present() override;

	void Begin(Camera* cam) override;
	void Submit(const List<Light*>& lights) override;
	void Submit(Entity3D* entity) override;
	void Submit(const RenderCommand& cmd);
	void Submit(Mesh* mesh, mat4 transform);
	void End() override;

	inline List<RenderCommand> GetCommandQueue() const { return commandQueue; }
};
}