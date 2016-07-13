#include "deferredrenderer.h"
#include <core/window.h>
#include <graphics/shader/shaderfactory.h>
#include <graphics/buffer/bufferlayout.h>

void DeferredRenderer::SetBlendingInternal(bool blending) {
	D3DContext::GetDeviceContext()->OMSetBlendState(blending ? blendState[1] : blendState[0], nullptr, 0xFFFFFFFF);
}

void DeferredRenderer::SetDepthInternal(bool depth) {
	D3DContext::GetDeviceContext()->OMSetDepthStencilState(depth ? depthState[1] : depthState[0], 0);
}

void DeferredRenderer::CreateDepthStates() {


	//DEFAULT
	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS;

	desc.StencilEnable = false;

	D3DContext::GetDevice()->CreateDepthStencilState(&desc, &depthState[1]);

	ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));


	//NO DEPTH

	ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	desc.DepthEnable = false;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D11_COMPARISON_EQUAL;

	desc.StencilEnable = false;

	D3DContext::GetDevice()->CreateDepthStencilState(&desc, &depthState[0]);
}

void DeferredRenderer::CreateBlendStates() {


	//DEFAULT
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));

	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = false;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3DContext::GetDevice()->CreateBlendState(&desc, &blendState[0]);


	// ENABLED

	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3DContext::GetDevice()->CreateBlendState(&desc, &blendState[1]);
}

void DeferredRenderer::CreateShaders() {
	BufferLayout render, composit;

	render.Push<vec3>("POSITION");
	render.Push<vec2>("TEXCOORDS");
	render.Push<vec3>("NORMALS");

	geometryShader = ShaderFactory::GetShader(FD_DEFERRED_SHADER_TYPE_GEOMETRY);

	render.CreateInputLayout(geometryShader);

	composit.Push<vec3>("POSITION");
	composit.Push<vec2>("TEXCOORDS");

	directionalpass = ShaderFactory::GetShader(FD_DEFERRED_SHADER_TYPE_DIRECTIONAL_LIGHT);

	composit.CreateInputLayout(directionalpass);


	constantBufferSlotCache[FD_DEFERRED_SHADER_SLOT_GEOMETRY_PROJECTION] = geometryShader->GetVSConstantBufferSlotByName("proj");
	constantBufferSlotCache[FD_DEFERRED_SHADER_SLOT_GEOMETRY_MODELDATA] = geometryShader->GetVSConstantBufferSlotByName("modelData");
	constantBufferSlotCache[FD_DEFERRED_SHADER_SLOT_GEOMETRY_MATERIALDATA] = geometryShader->GetPSConstantBufferSlotByName("materialData");
	constantBufferSlotCache[FD_DEFERRED_SHADER_SLOT_DIRECTIONAL_DATA] = directionalpass->GetPSConstantBufferSlotByName("lightData");
	//constantBufferSlotCache[FD_DEFERRED_SHADER_SLOT_POINT_DATA] = geometryShader->GetVSConstantBufferSlotByName("lightData");

}

DeferredRenderer::DeferredRenderer(unsigned int width, unsigned int height) {
	CreateDepthStates();
	CreateBlendStates();
	
	CreateShaders();

	mrt.Init(width, height, FD_TEXTURE_FORMAT_FLOAT_32_32_32_32, true);


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

	SetProjectionMatrix(mat4::Perspective(70.0f, (float)width / height, 0.001f, 1000.0f));
}

DeferredRenderer::~DeferredRenderer() {
	delete geometryShader;
	delete directionalpass;

	DX_FREE(depthState[0]);
	DX_FREE(blendState[0]);
	DX_FREE(depthState[1]);
	DX_FREE(blendState[1]);
}

void DeferredRenderer::SetProjectionMatrix(const mat4& matrix) {
	geometryShader->SetVSConstantBuffer(constantBufferSlotCache[FD_DEFERRED_SHADER_SLOT_GEOMETRY_PROJECTION], (void*)&matrix);
}

void DeferredRenderer::AddEntity(Entity* e) {
	entities.Push_back(e);
}

void DeferredRenderer::RemoveEntity(Entity* e) {
	entities.Remove(e);
}

void DeferredRenderer::AddLight(DirectionalLight* light) {
	directionalLights.Push_back(light);
}

void DeferredRenderer::AddLight(PointLight* light) {
	pointLights.Push_back(light);
}

void DeferredRenderer::Render() {
	SetBlendingInternal(false);
	SetDepthInternal(true);

	geometryShader->Bind();
	
	mrt.BindAsRenderTarget();
	D3DContext::Clear(4);

	for (size_t i = 0; i < entities.GetSize(); i++) {
		Entity& e = *entities[i];

		e.GetModel()->Bind();

		Material* mat = e.GetMaterial();

		rData.translation = mat4::Translate(e.GetPosition());
		rData.rotation = mat4::Rotate(e.GetRotation());

		cData.color = mat->GetDiffuseColor();

		geometryShader->SetVSConstantBuffer(constantBufferSlotCache[FD_DEFERRED_SHADER_SLOT_GEOMETRY_MODELDATA], &rData);
		geometryShader->SetPSConstantBuffer(constantBufferSlotCache[FD_DEFERRED_SHADER_SLOT_GEOMETRY_MATERIALDATA], &cData);
		geometryShader->SetTexture(0, mat->GetDiffuseTexture());

		D3DContext::GetDeviceContext()->DrawIndexed(e.GetModel()->GetIndexBuffer()->GetCount(), 0, 0);
	}

	D3DContext::SetRenderTarget(nullptr);

	

	((Texture2D*)mrt[0])->Bind(0);
	((Texture2D*)mrt[1])->Bind(1);
	((Texture2D*)mrt[2])->Bind(2);

	vertexBufferPlane->Bind();
	indexBufferPlane->Bind();
	
	unsigned int indexCount = indexBufferPlane->GetCount();

	directionalpass->Bind();

	DirectionalLight* light = directionalLights[0];

	directionalpass->SetPSConstantBuffer(constantBufferSlotCache[FD_DEFERRED_SHADER_SLOT_DIRECTIONAL_DATA], (void*)light);

	D3DContext::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);

	SetBlendingInternal(true);
	SetDepthInternal(false);

	size_t num = directionalLights.GetSize();

	for (size_t i = 1; i < num; i++) {
		DirectionalLight* light = directionalLights[i];

		directionalpass->SetPSConstantBuffer(constantBufferSlotCache[FD_DEFERRED_SHADER_SLOT_DIRECTIONAL_DATA], (void*)light);

		D3DContext::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
	}
	
	

}