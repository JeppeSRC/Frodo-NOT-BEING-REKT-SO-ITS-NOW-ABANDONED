#include "deferredrenderer.h"
#include <core/window.h>
#include <graphics/shader/shaderfactory.h>
#include <graphics/buffer/bufferlayout.h>

DeferredRenderer::DeferredRenderer(unsigned int width, unsigned int height) {

	IDXGIDevice* dev;
	D3DContext::GetDevice()->QueryInterface(__uuidof(IDXGIDevice), (void**)&dev);
	
	IDXGIAdapter* ad;
	dev->GetAdapter(&ad);

	DXGI_ADAPTER_DESC desc;
	ad->GetDesc(&desc);

	FD_INFO("GPU Info: ");

	char string[128];

	wcstombs(string, desc.Description, 128);

	FD_INFO("\t Description: \t %s", string);
	FD_INFO("\t VRAM: \t\t %u", desc.DedicatedVideoMemory);

	mrt.Init(width, height, FD_TEXTURE_FORMAT_FLOAT_32_32_32_32, true);

	BufferLayout render, composit;

	render.Push<vec3>("POSITION");
	render.Push<vec2>("TEXCOORDS");
	render.Push<vec3>("NORMALS");

	renderShader = ShaderFactory::GetDeferredRenderShader();

	render.CreateInputLayout(renderShader);

	mat4 proj = mat4::Perspective(70.0f, (float)width / height, 0.0001f, 1000.0f);

	renderShader->SetVSConstantBuffer(0, (void*)&proj);


	composit.Push<vec3>("POSITION");
	composit.Push<vec2>("TEXCOORDS");

	compositionShader = ShaderFactory::GetDeferredCompositionShader();

	composit.CreateInputLayout(compositionShader);


	struct PlaneVertex {
		vec3 position;
		vec2 texCoords;
	};

	PlaneVertex vertices[4]{
		{vec3(-1, 1, 0), vec2(0, 0)},
		{vec3(1, 1, 0), vec2(1, 0)},
		{vec3(1, -1, 0), vec2(1, 1)},
		{vec3(-1, -1, 0), vec2(0, 1)},
	};

	unsigned int indices[6]{0, 1, 2, 2, 3, 0};

	vertexBufferPlane = new VertexBuffer(vertices, sizeof(vertices), sizeof(PlaneVertex));
	indexBufferPlane = new IndexBuffer(indices, 6);
}

DeferredRenderer::~DeferredRenderer() {
	delete renderShader;
	delete compositionShader;
}

void DeferredRenderer::AddEntity(Entity* e) {
	entities.Push_back(e);
}

void DeferredRenderer::RemoveEntity(Entity* e) {
	entities.Remove(e);
}

void DeferredRenderer::Render() {
	renderShader->Bind();
	
	mrt.BindAsRenderTarget();
	D3DContext::Clear(4);

	for (size_t i = 0; i < entities.GetSize(); i++) {
		Entity& e = *entities[i];

		e.GetModel()->Bind();

		Material& mat = *e.GetMaterial();

		rData.translation = mat4::Translate(e.GetPosition());
		rData.rotation = mat4::Rotate(e.GetRotation());

		cData.color = mat.GetDiffuseColor();

		renderShader->SetVSConstantBuffer(1, &rData);
		renderShader->SetPSConstantBuffer(0, &cData);
		renderShader->SetTexture(0, TextureManager::Get("default"));

		D3DContext::GetDeviceContext()->DrawIndexed(e.GetModel()->GetIndexBuffer()->GetCount(), 0, 0);
	}

	D3DContext::SetRenderTarget(nullptr);

	compositionShader->Bind();

	vertexBufferPlane->Bind();
	indexBufferPlane->Bind();

	compositionShader->SetTexture(0, (Texture2D*)mrt[0]);
	compositionShader->SetTexture(1, (Texture2D*)mrt[1]);
	compositionShader->SetTexture(2, (Texture2D*)mrt[2]);

	D3DContext::GetDeviceContext()->DrawIndexed(indexBufferPlane->GetCount(), 0, 0);

}