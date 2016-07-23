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
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	desc.StencilEnable = false;

	D3DContext::GetDevice()->CreateDepthStencilState(&desc, &depthState[1]);

	ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));


	//NO DEPTH

	ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	desc.DepthEnable = false;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
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

	directionalLightShader = ShaderFactory::GetShader(FD_DEFERRED_SHADER_TYPE_DIRECTIONAL_LIGHT);
	pointLightShader = ShaderFactory::GetShader(FD_DEFERRED_SHADER_TYPE_POINT_LIGHT);

	composit.CreateInputLayout(directionalLightShader);
	composit.CreateInputLayout(pointLightShader);
	

	constantBufferSlotCache[FD_SLOT_GEOMETRY_PROJECTION] = geometryShader->GetVSConstantBufferSlotByName("projectionMatrix");
	constantBufferSlotCache[FD_SLOT_GEOMETRY_VIEW] = geometryShader->GetVSConstantBufferSlotByName("viewMatrix");
	constantBufferSlotCache[FD_SLOT_GEOMETRY_MODELDATA] = geometryShader->GetVSConstantBufferSlotByName("modelData");
	constantBufferSlotCache[FD_SLOT_GEOMETRY_MATERIALDATA] = geometryShader->GetPSConstantBufferSlotByName("materialData");
	constantBufferSlotCache[FD_SLOT_DIRECTIONAL_DATA] = directionalLightShader->GetPSConstantBufferSlotByName("lightData");
	constantBufferSlotCache[FD_SLOT_POINT_DATA] = pointLightShader->GetPSConstantBufferSlotByName("lightData");

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

	camera = new Camera(vec3(0, 0, 0));

	SetProjectionMatrix(mat4::Perspective(70.0f, (float)width / height, 0.001f, 1000.0f));

	D3DContext::GetDevice()->CreateDeferredContext(0, &deferredContext);
	D3DContext::SetActiveDeviceContext(deferredContext);
	D3DContext::SetViewPort(0, 0, 1000.0f, 600.0f);
	D3DContext::SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DContext::SetActiveDeviceContext(nullptr);

	deferredContext->FinishCommandList(FALSE, nullptr);
}

DeferredRenderer::~DeferredRenderer() {
	delete geometryShader;
	delete directionalLightShader;

	DX_FREE(depthState[0]);
	DX_FREE(blendState[0]);
	DX_FREE(depthState[1]);
	DX_FREE(blendState[1]);

	DX_FREE(deferredContext);
//	DX_FREE(list);
}

void DeferredRenderer::SetProjectionMatrix(const mat4& matrix) {
	geometryShader->SetVSConstantBuffer(constantBufferSlotCache[FD_SLOT_GEOMETRY_PROJECTION], (void*)&matrix);
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
	D3DContext::SetActiveDeviceContext(deferredContext);
	D3DContext::SetViewPort(0, 0, 1000.0f, 600.0f);
	D3DContext::SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	SetBlendingInternal(false);
	SetDepthInternal(true);

	geometryShader->SetVSConstantBuffer(constantBufferSlotCache[FD_SLOT_GEOMETRY_VIEW], (void*)&camera->GetViewMatrix());

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

		geometryShader->SetVSConstantBuffer(constantBufferSlotCache[FD_SLOT_GEOMETRY_MODELDATA], &rData);
		geometryShader->SetPSConstantBuffer(constantBufferSlotCache[FD_SLOT_GEOMETRY_MATERIALDATA], &cData);
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

	SetBlendingInternal(true);
	SetDepthInternal(false);

	directionalLightShader->Bind();

	size_t num = directionalLights.GetSize();

	for (size_t i = 0; i < num; i++) {
		directionalLightShader->SetPSConstantBuffer(constantBufferSlotCache[FD_SLOT_DIRECTIONAL_DATA], (void*)directionalLights[i]);

		D3DContext::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
	}
	
	pointLightShader->Bind();

	num = pointLights.GetSize();

	for (int i = 0; i < num; i++) {
		pointLightShader->SetPSConstantBuffer(constantBufferSlotCache[FD_SLOT_POINT_DATA], (void*)pointLights[i]);

		D3DContext::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
	}

	D3DContext::SetActiveDeviceContext(nullptr);
	deferredContext->FinishCommandList(FALSE, &list);
	D3DContext::GetDeviceContext()->ExecuteCommandList(list, TRUE);
	
	//DX_FREE(list);
}