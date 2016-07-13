#pragma once

#include <fd.h>
#include <graphics/d3dcontext.h>
#include <graphics/shader/shader.h>
#include <graphics/texture/framebuffer2d.h>
#include <entity/entity.h>

#define FD_RENDERER_MRT_INDEX_POSITIONS		0x0
#define FD_RENDERER_MRT_INDEX_DIFFUSE		0x1
#define FD_RENDERER_MRT_INDEX_NORMALS		0x2 
#define FD_RENDERER_MRT_INDEX_SHADOWMAP		0x3

class FDAPI DeferredRenderer {
private:

	struct RenderData {
		mat4 translation;
		mat4 rotation;
	} rData;


	struct CompositionData {
		vec4 color;
	}cData;

	VertexBuffer* vertexBufferPlane;
	IndexBuffer* indexBufferPlane;

private:
	FramebufferMRT<4> mrt;

	Shader* renderShader;
	Shader* directionalpass;

	List<Entity*> entities;

public:
	DeferredRenderer(unsigned int width, unsigned int height);
	~DeferredRenderer();

	void AddEntity(Entity* e);
	void RemoveEntity(Entity* e);

	void Render();
};
