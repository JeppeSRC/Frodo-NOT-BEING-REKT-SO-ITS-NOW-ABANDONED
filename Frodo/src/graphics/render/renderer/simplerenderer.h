#pragma once

#include "renderer.h"
#include <graphics/texture/framebuffer2d.h>
#include <graphics/texture/shadowmap2d.h>
#include <graphics/texture/shadowmapcube.h>
#include <graphics/texture/framebuffercube.h>

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

	class SR_Light {
	public:
		Light* light;
		Shader* shader;
		Shader* shadowShader;
		Framebuffer* shadowMap;
		
		mat4 projection;

		virtual void SetupShadowShader(Entity3D* e, Camera* camera) const = 0;
		virtual void SetupShader(Entity3D* e, Camera* camera) const = 0;
	};

	class SR_DirectionalLight : public SR_Light {
	private:
		mutable mat4 lightMatrix;
	public:
		void SetupShadowShader(Entity3D* e, Camera* camera) const override;
		void SetupShader(Entity3D* e, Camera* camera) const override;
	};

	class SR_PointLight : public SR_Light {
	public:
		float maxDepth;

		void SetupShadowShader(Entity3D* e, Camera* camera) const override;
		void SetupShader(Entity3D* e, Camera* camera) const override;
	};

private:
	Camera* camera;

	Shader* shadowShader2D;
	Shader* shadowShaderCube;
	Shader* pointShader;
	Shader* pointShaderShadow;
	Shader* directionalShader;
	Shader* directionalShaderShadow;

	Framebuffer* shadowMap2D;
	Framebuffer* shadowMapCube;

	Material* baseMaterial;
	
	Shader::ConstantBufferSlot cameraBuffer;

	ID3D11DepthStencilState* depthState[2];
	ID3D11BlendState* blendState[2];

	List<SR_Light*> lights;
	List<Entity3D*> entities;
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
	void Submit(Entity3D* entity) override;
	void Remove(Light* light);
	void Remove(Entity3D* entity);
	void Present() override;

	inline Material* GetBaseMaterial() const { return baseMaterial; }

	inline List<Entity3D*>& GetEntities() { return entities; }
	inline List<SR_Light*>& GetLights() { return lights; }

};

}
