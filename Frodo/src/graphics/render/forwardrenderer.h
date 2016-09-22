#pragma once
#include <fd.h>
#include "renderer.h"
#include <util/list.h>
#include <graphics/shader/shader.h>
#include <graphics/shader/shaderfactory.h>

class FDAPI ForwardRenderer : public Renderer {
private:
	List<Entity*> entities;
	List<DirectionalLight*> directionalLights;
	List<PointLight*> pointLights;
	List<SpotLight*> spotLights;

	Shader* directionalLightShader;
	Shader* pointLightShader;
	Shader* spotLightShader;

	ID3D11BlendState* blendState[2];
	ID3D11DepthStencilState* depthState[2];

	bool oneLightRendered;
	bool geometryRendered;

private:
	void CreateShaders();

	void CreateBlendStates();
	void CreateDepthStates();


	void SetBlendingInternal(bool blend);
	void SetDepthInternal(bool depth);

	enum {
		FD_SLOT_DIRECTIONAL_VIEW_DATA,
		FD_SLOT_DIRECTIONAL_MODEL_DATA,
		FD_SLOT_DIRECTIONAL_LIGHT_DATA,
		FD_SLOT_DIRECTIONAL_MATERIAL_DATA,

		FD_SLOT_POINT_VIEW_DATA,
		FD_SLOT_POINT_MODEL_DATA,
		FD_SLOT_POINT_LIGHT_DATA,
		FD_SLOT_POINT_MATERIAL_DATA,

		FD_SLOT_SPOT_VIEW_DATA,
		FD_SLOT_SPOT_MODEL_DATA,
		FD_SLOT_SPOT_LIGHT_DATA,
		FD_SLOT_SPOT_MATERIAL_DATA,

		FD_SLOT_NUM
	};

	unsigned int slotCache[FD_SLOT_NUM];

private:

	struct VIEW_DATA {
		mat4 projection;
		mat4 view;
	} view_data;

	struct MODEL_DATA {
		mat4 translation;
		mat4 roatation;
		mat4 scale;
	} model_data; 

	struct MATERIAL_DATA {
		vec4 color;
	} material_data;

public:
	ForwardRenderer(Window* window);
	~ForwardRenderer();

	void Add(Entity* entity) override;
	void Add(Light* light) override;

	void Remove(Entity* entity) override;
	void Remove(Light* entity) override;

	void Render() override;
};

