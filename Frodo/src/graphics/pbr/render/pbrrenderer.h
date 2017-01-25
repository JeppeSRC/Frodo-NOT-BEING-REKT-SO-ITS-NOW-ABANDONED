#pragma once

#include <fd.h>
#include <graphics/render/light/light.h>
#include <graphics/shader/shader.h>
#include <graphics/render/renderer/renderer.h>
#include <graphics/render/camera/camera.h>
#include <entity/entity.h>


class FDAPI PBRStaticRenderer : public Renderer {
private:
	Shader::ConstantBufferSlot light;
	Shader::ConstantBufferSlot camera;


public:
	PBRStaticRenderer(Window* window);
	~PBRStaticRenderer();

	virtual void Present() override;

	void Begin(Camera* cam) override;
	void Submit(const LightStack& lights) override;
	void End() override;
};