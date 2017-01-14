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
	directionalLightShader->ShaderGenComplete();
	layout.CreateInputLayout(directionalLightShader);

	pointLightShader = ShaderFactory::GetShader(FD_FORWARD_SHADER_POINT_LIGHT);
	pointLightShader->ShaderGenComplete();
	layout.CreateInputLayout(pointLightShader);

	spotLightShader = ShaderFactory::GetShader(FD_FORWARD_SHADER_SPOT_LIGHT);
	spotLightShader->ShaderGenComplete();
	layout.CreateInputLayout(spotLightShader);
}

void ForwardRenderer::CreateBlendStates() {
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));

	desc.RenderTarget[0].BlendEnable = false;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3DContext::GetDevice()->CreateBlendState(&desc, &blendState[0]);

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

void ForwardRenderer::CreateDepthStates() {
	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	desc.StencilEnable = false;

	D3DContext::GetDevice()->CreateDepthStencilState(&desc, &depthState[0]);

	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_EQUAL;
	desc.StencilEnable = false;

	D3DContext::GetDevice()->CreateDepthStencilState(&desc, &depthState[1]);

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

	slotCache[FD_SLOT_SPOT_VIEW_DATA] = spotLightShader->GetVSConstantBufferSlotByName("viewData");
	slotCache[FD_SLOT_SPOT_MODEL_DATA] = spotLightShader->GetVSConstantBufferSlotByName("modelData");
	slotCache[FD_SLOT_SPOT_LIGHT_DATA] = spotLightShader->GetPSConstantBufferSlotByName("lightData");
	slotCache[FD_SLOT_SPOT_MATERIAL_DATA] = spotLightShader->GetPSConstantBufferSlotByName("materialData");

}


ForwardRenderer::~ForwardRenderer(){
	delete directionalLightShader;
	delete pointLightShader;

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
	geometryRendered = false;
	
	uint_t numl = directionalLights.GetSize();
	uint_t nume = entities.GetSize();


	viewData.projection = camera->GetProjectionMatrix();
	viewData.view = camera->GetViewMatrix();

	directionalLightShader->SetVSConstantBuffer(slotCache[FD_SLOT_DIRECTIONAL_VIEW_DATA], (void*)&viewData);
	pointLightShader->SetVSConstantBuffer(slotCache[FD_SLOT_POINT_VIEW_DATA], (void*)&viewData);
	spotLightShader->SetVSConstantBuffer(slotCache[FD_SLOT_SPOT_VIEW_DATA], (void*)&viewData);

	directionalLightShader->Bind();
	
//	Directional Lights

	if (numl > 0) {
		for (uint_t l = 0; l < numl; l++) {
			directionalLightShader->SetPSConstantBuffer(slotCache[FD_SLOT_DIRECTIONAL_LIGHT_DATA], (void*)directionalLights[l]);
			for (uint_t i = 0; i < nume; i++) {
				Entity& e = *entities[i];
				Material& mat = *e.GetMaterial();

				materialData.color = mat.GetDiffuseColor();

				modelData.translation = mat4::Translate(e.GetPosition());
				modelData.roatation = mat4::Rotate(e.GetRotation());
				modelData.scale = mat4::Scale(e.GetScale());

				e.GetModel()->Bind();

				directionalLightShader->SetPSConstantBuffer(slotCache[FD_SLOT_DIRECTIONAL_MATERIAL_DATA], (void*)&materialData);
				directionalLightShader->SetVSConstantBuffer(slotCache[FD_SLOT_DIRECTIONAL_MODEL_DATA], (void*)&modelData);


				directionalLightShader->SetTexture(0, mat.GetDiffuseTexture());

				D3DContext::GetDeviceContext()->DrawIndexed(e.GetModel()->GetIndexBuffer()->GetCount(), 0, 0);
			}

			FD_ENABLE_BLEDNING
		}
	}

	
	
	numl = pointLights.GetSize();
	
	if (numl > 0) {
		pointLightShader->Bind();
	
		for (uint_t i = 0; i < nume; i++) {
			if (!geometryRendered) { FD_DISABLE_BLENDING }
			Entity& e = *entities[i];
			Material& mat = *e.GetMaterial();

			materialData.color = mat.GetDiffuseColor();

			modelData.translation = mat4::Translate(e.GetPosition());
			modelData.roatation = mat4::Rotate(e.GetRotation());
			modelData.scale = mat4::Scale(e.GetScale());

			e.GetModel()->Bind();

			pointLightShader->SetPSConstantBuffer(slotCache[FD_SLOT_POINT_MATERIAL_DATA], (void*)&materialData);
			pointLightShader->SetVSConstantBuffer(slotCache[FD_SLOT_POINT_MODEL_DATA], (void*)&modelData);

			pointLightShader->SetTexture(0, mat.GetDiffuseTexture());

			uint32 indexCount = e.GetModel()->GetIndexBuffer()->GetCount();

			for (uint_t l = 0; l < numl; l++) {
				pointLightShader->SetPSConstantBuffer(slotCache[FD_SLOT_POINT_LIGHT_DATA], (void*)pointLights[l]);

				D3DContext::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
				
				FD_ENABLE_BLEDNING
			}
		}

		geometryRendered = true;
	}

	numl = spotLights.GetSize();

	if (numl > 0) {

		spotLightShader->Bind();

		for (uint_t i = 0; i < nume; i++) {
			if (!geometryRendered) { FD_DISABLE_BLENDING }
			Entity& e = *entities[i];
			Material& mat = *e.GetMaterial();

			materialData.color = mat.GetDiffuseColor();

			modelData.translation = mat4::Translate(e.GetPosition());
			modelData.roatation = mat4::Rotate(e.GetRotation());
			modelData.scale = mat4::Scale(e.GetScale());

			e.GetModel()->Bind();

			spotLightShader->SetPSConstantBuffer(slotCache[FD_SLOT_SPOT_MATERIAL_DATA], (void*)&materialData);
			spotLightShader->SetVSConstantBuffer(slotCache[FD_SLOT_SPOT_MODEL_DATA], (void*)&modelData);

			spotLightShader->SetTexture(0, mat.GetDiffuseTexture());

			uint32 indexCount = e.GetModel()->GetIndexBuffer()->GetCount();

			for (uint_t l = 0; l < numl; l++) {
				spotLightShader->SetPSConstantBuffer(slotCache[FD_SLOT_SPOT_LIGHT_DATA], (void*)spotLights[l]);

				D3DContext::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);

				FD_ENABLE_BLEDNING
			}
		}
	}
}