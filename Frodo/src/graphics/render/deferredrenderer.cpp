#include "deferredrenderer.h"
#include <core/window.h>
#include <graphics/shader/shaderfactory.h>
#include <graphics/buffer/bufferlayout.h>

DeferredRenderer::DeferredRenderer(unsigned int width, unsigned int height) {

	mrt.Init(width, height, FD_TEXTURE_FORMAT_FLOAT_32_32_32_32, true);

	BufferLayout render;

	render.Push<vec3>("POSITION");
	render.Push<vec2>("TEXCOORDS");
	render.Push<vec3>("NORMALS");

	renderShader = ShaderFactory::GetDeferredRenderShader();

	render.CreateInputLayout(renderShader);

	mat4 proj = mat4::Perspective(70.0f, (float)width / height, 0.0001f, 1000.0f);

	renderShader->SetVSConstantBuffer(0, (void*)&proj);

}

DeferredRenderer::~DeferredRenderer() {
	delete renderShader;
}

void DeferredRenderer::AddEntity(Entity* e) {
	entities.Push_back(e);
}

void DeferredRenderer::RemoveEntity(Entity* e) {
	entities.Remove(e);
}

void DeferredRenderer::Render() {
	renderShader->Bind();
	
	for (size_t i = 0; i < entities.GetSize(); i++) {
		Entity& e = *entities[i];

		e.GetModel()->Bind();

		Material* mat = e.GetMaterial();

		struct data {
			mat4 translation;
			mat4 rotation;
			vec4 color;
		} t;

		t.translation = mat4::Translate(e.GetPosition());
		t.rotation = mat4::Rotate(e.GetRotation());
		t.color = mat->GetDiffuseColor();

		renderShader->SetVSConstantBuffer(1, &t);

		D3DContext::GetDeviceContext()->DrawIndexed(e.GetModel()->GetIndexBuffer()->GetCount(), 0, 0);
	}

}