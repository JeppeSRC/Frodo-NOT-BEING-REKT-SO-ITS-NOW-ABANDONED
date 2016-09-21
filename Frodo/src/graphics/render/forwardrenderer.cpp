#include "forwardrenderer.h"
#include <graphics/buffer/bufferlayout.h>

#define FD_ENABLE_BLEDNING \
if (!oneLightRendered) { \
	oneLightRendered = true; \
	SetBlendingInternal(true); \
	SetDepthInternal(false); \
}

#define FD_DISABLE_BLENDING \
oneLightRendered = false; \
SetBlendingInternal(false); \
SetDepthInternal(true); \

void ForwardRenderer::CreateShaders() {
	BufferLayout layout;

	layout.Push<vec3>("POSITION");
	layout.Push<vec2>("TEXCOORDS");
	layout.Push<vec3>("NORMALS");

	directionalLightShader = ShaderFactory::GetShader(FD_FORWARD_SHADER_DIRECTIONAL_LIGHT);
	layout.CreateInputLayout(directionalLightShader);

	pointLightShader = ShaderFactory::GetShader(FD_FORWARD_SHADER_POINT_LIGHT);
	layout.CreateInputLayout(pointLightShader);
}

void ForwardRenderer::CreateBlendStates() {
	D3D11_BLEND_DESC d;
	ZeroMemory(&d, sizeof(D3D11_BLEND_DESC));

	d.RenderTarget[0].BlendEnable = false;
	d.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3DContext::GetDevice()->CreateBlendState(&d, &blendState[0]);

	d.RenderTarget[0].BlendEnable = true;
	d.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	d.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_ALPHA;
	d.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	d.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	d.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	d.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	d.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3DContext::GetDevice()->CreateBlendState(&d, &blendState[1]);

}

void ForwardRenderer::CreateDepthStates() {
	D3D11_DEPTH_STENCIL_DESC d;
	ZeroMemory(&d, sizeof(D3D11_DEPTH_STENCIL_DESC));

	d.DepthEnable = true;
	d.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	d.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	d.StencilEnable = false;

	D3DContext::GetDevice()->CreateDepthStencilState(&d, &depthState[0]);

	d.DepthEnable = true;
	d.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	d.DepthFunc = D3D11_COMPARISON_EQUAL;
	d.StencilEnable = false;

	D3DContext::GetDevice()->CreateDepthStencilState(&d, &depthState[1]);

}

void ForwardRenderer::SetBlendingInternal(bool blend) {
	D3DContext::GetDeviceContext()->OMSetBlendState(blendState[blend ? 1 : 0], nullptr, 0xFFFFFFFF);
}

void ForwardRenderer::SetDepthInternal(bool depth) {
	D3DContext::GetDeviceContext()->OMSetDepthStencilState(depthState[depth ? 0 : 1], 0);
}

ForwardRenderer::ForwardRenderer(Window* window) : Renderer(window, nullptr), oneLightRendered(false) {
	CreateShaders();

	CreateBlendStates();
	CreateDepthStates();

	slotCache[FD_SLOT_DIRECTIONAL_VIEW_DATA] = directionalLightShader->GetVSConstantBufferSlotByName("viewData");
	slotCache[FD_SLOT_DIRECTIONAL_MODEL_DATA] = directionalLightShader->GetVSConstantBufferSlotByName("modelData");
	slotCache[FD_SLOT_DIRECTIONAL_LIGHT_DATA] = directionalLightShader->GetPSConstantBufferSlotByName("lightData");
	slotCache[FD_SLOT_DIRECTIONAL_MATERIAL_DATA] = directionalLightShader->GetPSConstantBufferSlotByName("materialData");

	slotCache[FD_SLOT_POINT_VIEW_DATA] = pointLightShader->GetVSConstantBufferSlotByName("viewData");
	slotCache[FD_SLOT_POINT_MODEL_DATA] = pointLightShader->GetVSConstantBufferSlotByName("modelData");
	slotCache[FD_SLOT_POINT_LIGHT_DATA] = pointLightShader->GetPSConstantBufferSlotByName("lightData");
	slotCache[FD_SLOT_POINT_MATERIAL_DATA] = pointLightShader->GetPSConstantBufferSlotByName("materialData");
}


ForwardRenderer::~ForwardRenderer(){
	delete directionalLightShader;

	DX_FREE(depthState[0]);
	DX_FREE(depthState[1]);
	DX_FREE(blendState[0]);
	DX_FREE(blendState[1]);
}

void ForwardRenderer::Add(Entity* entity) { 
	entities.Push_back(entity);
}

void ForwardRenderer::Add(Light* light) { 
	switch (light->GetLightType()) {
		case FD_LIGHT_TYPE_DIRECTIONAL:
			directionalLights.Push_back((DirectionalLight*)light);
			break;
		case FD_LIGHT_TYPE_POINT:
			pointLights.Push_back((PointLight*)light);
			break;
		case FD_LIGHT_TYPE_SPOT:
			spotLights.Push_back((SpotLight*)light);
			break;
		case FD_LIGHT_TYPE_NONE:
			FD_WARNING("Light.lightType = FD_LIGHT_TYPE_NONE does nothing (%s)", __FUNCSIG__);
			break;
	}
}

void ForwardRenderer::Remove(Entity* entity) { 
	entities.Remove(entity);
}

void ForwardRenderer::Remove(Light* light) { 
	switch (light->GetLightType()) {
		case FD_LIGHT_TYPE_DIRECTIONAL:
			directionalLights.Remove((DirectionalLight*)light);
			break;
		case FD_LIGHT_TYPE_POINT:
			pointLights.Remove((PointLight*)light);
			break;
		case FD_LIGHT_TYPE_SPOT:
			spotLights.Remove((SpotLight*)light);
			break;
		case FD_LIGHT_TYPE_NONE:
			FD_WARNING("Light.lightType = FD_LIGHT_TYPE_NONE does nothing (%s)", __FUNCSIG__);
			break;
	}
}


void ForwardRenderer::Render() {
	FD_DISABLE_BLENDING
	
	size_t numl = directionalLights.GetSize();
	size_t nume = entities.GetSize();

//	directionalLightShader->Bind();

	view_data.projection = camera->GetProjectionMatrix();
	view_data.view = camera->GetViewMatrix();

	//directionalLightShader->SetVSConstantBuffer(slotCache[FD_SLOT_DIRECTIONAL_VIEW_DATA], (void*)&view_data);
	pointLightShader->SetVSConstantBuffer(slotCache[FD_SLOT_POINT_VIEW_DATA], (void*)&view_data);

/*
	//Directional Lights
	for (size_t l = 0; l < numl; l++) {
		directionalLightShader->SetPSConstantBuffer(slotCache[FD_SLOT_DIRECTIONAL_LIGHT_DATA], (void*)directionalLights[l]);
		for (size_t i = 0; i < nume; i++) {
			Entity& e = *entities[i];
			Material& mat = *e.GetMaterial();

			material_data.color = mat.GetDiffuseColor();

			model_data.translation = mat4::Translate(e.GetPosition());
			model_data.roatation = mat4::Rotate(e.GetRotation());
			model_data.scale = mat4::Scale(e.GetScale());

			e.GetModel()->Bind();

			directionalLightShader->SetPSConstantBuffer(slotCache[FD_SLOT_DIRECTIONAL_MATERIAL_DATA], (void*)&material_data);
			directionalLightShader->SetVSConstantBuffer(slotCache[FD_SLOT_DIRECTIONAL_MODEL_DATA], (void*)&model_data);
			

			directionalLightShader->SetTexture(0, mat.GetDiffuseTexture());

			D3DContext::GetDeviceContext()->DrawIndexed(e.GetModel()->GetIndexBuffer()->GetCount(), 0, 0);
		}

		FD_ENABLE_BLEDNING
	}
	*/
	
	numl = pointLights.GetSize();

	if (numl > 0) {
		pointLightShader->Bind();
	
		for (size_t i = 0; i < nume; i++) {
			FD_DISABLE_BLENDING
			Entity& e = *entities[i];
			Material& mat = *e.GetMaterial();

			material_data.color = mat.GetDiffuseColor();

			model_data.translation = mat4::Translate(e.GetPosition());
			model_data.roatation = mat4::Rotate(e.GetRotation());
			model_data.scale = mat4::Scale(e.GetScale());

			e.GetModel()->Bind();

			pointLightShader->SetPSConstantBuffer(slotCache[FD_SLOT_POINT_MATERIAL_DATA], (void*)&material_data);
			pointLightShader->SetVSConstantBuffer(slotCache[FD_SLOT_POINT_MODEL_DATA], (void*)&model_data);

			pointLightShader->SetTexture(0, mat.GetDiffuseTexture());
		
			for (size_t l = 0; l < numl; l++) {
				pointLightShader->SetPSConstantBuffer(slotCache[FD_SLOT_POINT_LIGHT_DATA], (void*)pointLights[l]);

				D3DContext::GetDeviceContext()->DrawIndexed(e.GetModel()->GetIndexBuffer()->GetCount(), 0, 0);
				FD_ENABLE_BLEDNING
			}
		}
	}
}