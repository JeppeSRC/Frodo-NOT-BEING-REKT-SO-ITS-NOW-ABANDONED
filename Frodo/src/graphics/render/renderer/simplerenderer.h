#pragma once

#include "renderer.h"
#include <graphics/texture/framebuffer2d.h>

namespace FD {

class FDAPI SimpleRenderer : public Renderer {
private:
	enum FD_RENDERER_DEPTH_STATE {
		FD_RENDERER_DEPTH_DEFAULT,
		FD_RENDERER_DEPTH_EQUAL,
		FD_RENDERER_DEPTH_NUM_STATES
	};

	enum FD_RENDERER_BLEND_STATE {
		FD_RENDERER_BLEND_DEFAULT,
		FD_RENDERER_BLEND_ENABLED,
		FD_RENDERER_BLEND_NUM_STATES
	};
private:
	Mesh* plane;
	FramebufferMRT<3> renderTarget;

	Shader* geometryShader;
	Shader* pointShader;
	
	Shader::ConstantBufferSlot camera;

	ID3D11DepthStencilState* depthState[2];
	ID3D11BlendState* blendState[2];

	List<PointLight*> lights;

private:
	void CreateDepthAndBlendStates();
	void InitializeShaders();

	void SetDepth(FD_RENDERER_DEPTH_STATE state);
	void SetBlend(FD_RENDERER_BLEND_STATE state);
public:
	SimpleRenderer(Window* window);
	~SimpleRenderer();

	void Begin(Camera* camera) override;
	void Submit(Light* light) override;
	using Renderer::Submit;
	void End() override;
	void Present() override;

	inline Shader* GetGeometryShader() const { return geometryShader; }
};

}
