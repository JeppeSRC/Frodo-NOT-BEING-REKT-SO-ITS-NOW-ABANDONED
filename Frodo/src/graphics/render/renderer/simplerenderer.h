#pragma once

#include "renderer.h"

namespace FD {

class FDAPI SimpleRenderer : public Renderer {
private:
	Shader* geometryShader;
	Shader* pointShader;
	
	Shader::ConstantBufferSlot camera;

	ID3D11DepthStencilState* depthState[2];
	ID3D11BlendState* blendState[2];

	List<PointLight*> lights;

private:
	void CreateDepthAndBlendStates();
	void InitializeShaders();

public:
	SimpleRenderer(Window* window);
	~SimpleRenderer();

	void Begin(Camera* camera) override;
	void Submit(const LightStack& lightStack) override;
	void Submit(Light* light) override;
	void End() override;
	void Present() override;

	inline Shader* GetGeometryShader() const { return geometryShader; }
};

}
