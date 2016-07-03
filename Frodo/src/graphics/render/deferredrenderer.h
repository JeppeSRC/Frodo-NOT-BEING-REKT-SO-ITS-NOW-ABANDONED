#pragma once

#include <fd.h>
#include <graphics/d3dcontext.h>
#include <graphics/shader/shader.h>
#include <graphics/texture/framebuffer2d.h>
#include <entity/entity.h>

#define FD_RENDERER_MRT_INDEX_POSITIONS 0x0
#define FD_RENDERER_MRT_INDEX_COLORS	0x1
#define FD_RENDERER_MRT_INDEX_TEXCOORDS 0x2
#define FD_RENDERER_MRT_INDEX_NORMALS	0x3
#define FD_RENDERER_MRT_INDEX_DEPTH		0x4

class FDAPI DeferredRenderer {
private:

	struct RenderData {
		mat4 translation;
		mat4 rotation;
		vec4 color;
	} rData;


	struct CompositionData {
		
	}cData;

private:
	FramebufferMRT<4> mrt;

	Shader* renderShader;
	Shader* compositionShader;

	List<Entity*> entities;

public:
	DeferredRenderer(unsigned int width, unsigned int height);
	~DeferredRenderer();

	void AddEntity(Entity* e);
	void RemoveEntity(Entity* e);

	void Render();
};
