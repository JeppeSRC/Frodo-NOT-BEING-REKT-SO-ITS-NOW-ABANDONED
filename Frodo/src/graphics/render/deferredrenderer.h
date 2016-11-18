#pragma once

#include <fd.h>
#include <graphics/d3dcontext.h>
#include <graphics/render/camera/camera.h>
#include <graphics/shader/shader.h>
#include <graphics/texture/framebuffer2d.h>
#include <graphics/render/light/light.h>
#include <entity/entity.h>
#include "renderer.h"

#define FD_RENDERER_MRT_INDEX_POSITIONS		0x0
#define FD_RENDERER_MRT_INDEX_DIFFUSE		0x1
#define FD_RENDERER_MRT_INDEX_NORMALS		0x2 
#define FD_RENDERER_MRT_INDEX_SHADOWMAP		0x3

class FDAPI DeferredRenderer : public Renderer {
private:

	struct RenderData {
		mat4 translation;
		mat4 rotation;
		mat4 scale;
	} rData;


	struct CompositionData {
		vec4 color;
	}cData;

	VertexBuffer* vertexBufferPlane;
	IndexBuffer* indexBufferPlane;

private:
	FramebufferMRT<4> mrt;

	Shader* geometryShader;
	Shader* directionalLightShader;
	Shader* pointLightShader;
	Shader* spotLightShader;

	enum {
		FD_SLOT_GEOMETRY_PROJECTION,
		FD_SLOT_GEOMETRY_VIEW,
		FD_SLOT_GEOMETRY_MODELDATA,
		FD_SLOT_GEOMETRY_MATERIALDATA,
		FD_SLOT_DIRECTIONAL_DATA,
		FD_SLOT_POINT_DATA,
		FD_SLOT_SPOT_DATA,
		FD_SLOT_NUM
	};

	unsigned int constantBufferSlotCache[FD_SLOT_NUM];

	ID3D11DepthStencilState* depthState[2];
	ID3D11BlendState* blendState[2];

	List<Entity*> entities;
	List<DirectionalLight*> directionalLights;
	List<PointLight*> pointLights;
	List<SpotLight*> spotLights;

	void SetBlendingInternal(bool blending);
	void SetDepthInternal(bool depth);

	void CreateBlendStates();
	void CreateDepthStates();

	void CreateShaders();

public:
	DeferredRenderer(Window* window);
	~DeferredRenderer();

	void SetProjectionMatrix(const mat4& matrix);

	void Add(Entity* e) override;
	void Add(Light* light) override;

	void Remove(Entity* e) override;
	void Remove(Light* light) override;

	void Render();

	bool OnWindowActionResize(ivec2 size) override;
};
