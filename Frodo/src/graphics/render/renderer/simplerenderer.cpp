#include "simplerenderer.h"
#include <graphics/render/mesh/meshfactory.h>
#include <graphics/debug/debug.h>

static const char* lVertexShader =
#include <graphics/shader/shaders/forward/lVertex.hlsl>
;

static const char* pPixelShader =
#include <graphics/shader/shaders/forward/pPixel.hlsl> 
;

static const char* dPixelShader =
#include<graphics/shader/shaders/forward/dPixel.hlsl>
;

static const char* shadowMapVertex =
#include <graphics/shader/shaders/forward/shadowPassVertex.hlsl>
;

static const char* shadowMapPixel =
#include <graphics/shader/shaders/forward/shadowPassPixel.hlsl>
;


//TODO: Input layout from source
namespace FD {

void SimpleRenderer::CreateDepthAndBlendStates() {

	D3D11_DEPTH_STENCIL_DESC d = { 0 };

	//Default
	d.DepthEnable = true;
	d.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	d.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	d.StencilEnable = false;

	D3DContext::GetDevice()->CreateDepthStencilState(&d, &depthState[FD_RENDERER_DEPTH_DEFAULT]);

	//Light pass
	d.DepthFunc = D3D11_COMPARISON_EQUAL;

	D3DContext::GetDevice()->CreateDepthStencilState(&d, &depthState[FD_RENDERER_DEPTH_EQUAL]);


	D3D11_BLEND_DESC b = { 0 };

	//Default
	b.AlphaToCoverageEnable = false;
	b.IndependentBlendEnable = false; 

	b.RenderTarget[0].BlendEnable = false;
	b.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	b.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	b.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	b.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	b.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	b.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	b.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3DContext::GetDevice()->CreateBlendState(&b, &blendState[FD_RENDERER_BLEND_DEFAULT]);

	//Light pass
	b.RenderTarget[0].BlendEnable = true;

	D3DContext::GetDevice()->CreateBlendState(&b, &blendState[FD_RENDERER_BLEND_ENABLED]);
}

void SimpleRenderer::InitializeShaders() {
	pointShader = new Shader(lVertexShader, pPixelShader, true);
	pointShader->ShaderGenComplete();

	directionalShader = new Shader(lVertexShader, dPixelShader, true);
	directionalShader->ShaderGenComplete();

	shadowShader = new Shader(shadowMapVertex, shadowMapPixel, true);
	shadowShader->ShaderGenComplete();

	BufferLayout layout;

	layout.Push<vec3>("POSITION");
	layout.Push<vec3>("NORMAL");
	layout.Push<vec2>("TEXCOORD");

	layout.CreateInputLayout(pointShader);
	layout.CreateInputLayout(directionalShader);
	layout.CreateInputLayout(shadowShader);

	baseMaterial = new Material(pointShader);
}

void SimpleRenderer::SetDepth(FD_RENDERER_DEPTH_STATE state) {
	FD_ASSERT(state >= FD_RENDERER_DEPTH_NUM_STATES);
	D3DContext::GetDeviceContext()->OMSetDepthStencilState(depthState[state], 0);
}

float factor[4]{ 1.0f, 1.0f, 1.0f, 1.0f };

void SimpleRenderer::SetBlend(FD_RENDERER_BLEND_STATE state) {
	FD_ASSERT(state >= FD_RENDERER_BLEND_NUM_STATES);
	D3DContext::GetDeviceContext()->OMSetBlendState(blendState[state], factor, 0xFFFFFFFF);
}

SimpleRenderer::SimpleRenderer(Window* window) : Renderer(window) {
	CreateDepthAndBlendStates();
	InitializeShaders();

	cameraBuffer = pointShader->GetVSConstantBufferInfo("Camera");
	cameraBuffer.data = new byte[cameraBuffer.structSize];
	
//	shadowMap = new Framebuffer2D(4096, 4096, FD_TEXTURE_FORMAT_FLOAT_D32);
	shadowMap2D = new ShadowMap2D(2048, 2048);
	shadowMapCube = new ShadowMapCube(2048, 2048);
}

SimpleRenderer::~SimpleRenderer() {
	delete pointShader;
	delete directionalShader;
	delete shadowMap2D;
	delete shadowMapCube;
	delete baseMaterial;

	DX_FREE(depthState[0]);
	DX_FREE(depthState[1]);
	DX_FREE(blendState[0]);
	DX_FREE(blendState[1]);
}

void SimpleRenderer::Begin(Camera* camera) {
	this->camera = camera;
	this->cameraBuffer.SetElement("c_Position", (void*)&camera->GetPosition());
	this->cameraBuffer.SetElement("c_ViewMatrix", (void*)camera->GetViewMatrix().GetData());
	this->cameraBuffer.SetElement("c_ProjectionMatrix", (void*)camera->GetProjectionMatrix().GetData());
}

void SimpleRenderer::Submit(Light* light) {
	if (light->GetLightType() & FD_LIGHT_TYPE_POINT) {
		SR_PointLight* l = new SR_PointLight;
		l->light = light;
		l->shader = pointShader;
		l->shadowMap = shadowMapCube;
		lights.Push_back(l);
	} else if (light->GetLightType() & FD_LIGHT_TYPE_DIRECTIONAL) {
		SR_DirectionalLight* l = new SR_DirectionalLight;
		l->light = light;
		l->shader = directionalShader;
		l->shadowMap = shadowMap2D;
		l->projection = mat4::Orthographic(-20, 20, 20, -20, -20.0f, 20);
		lights.Push_back(l);
	}
}

void SimpleRenderer::Submit(Entity3D* entity) {
	entities.Push_back(entity);
}

void SimpleRenderer::Remove(Light* light) {
	auto cmp_func = [](SR_Light* item, Light* light) -> bool { if (light == item->light) return true; return false; };
	uint_t index = lights.Find<Light*>(cmp_func, light);
	lights.RemoveIndex(index);
}

void SimpleRenderer::Remove(Entity3D* entity) {
	entities.Remove(entity);
}


void SimpleRenderer::Present() {
	uint_t numLights = lights.GetSize();
	if (numLights == 0) return;
	SetDepth(FD_RENDERER_DEPTH_DEFAULT);
	SetBlend(FD_RENDERER_BLEND_DEFAULT);

	pointShader->SetVSConstantBuffer(cameraBuffer);
	directionalShader->SetVSConstantBuffer(cameraBuffer);

	uint_t numEntities = entities.GetSize();
	SR_Light* light = lights[0];
	if (light->light->GetLightType() & FD_LIGHT_CAST_SHADOW) {
		shadowShader->Bind();
		light->shadowMap->BindAsRenderTarget();
		D3DContext::Clear();

		mat4 lightMatrix = light->GetLightMatrix();

		for (uint_t i = 0; i < numEntities; i++) {
			Entity3D* entity = entities[i];
			shadowShader->SetVSConstantBuffer("MVP", (lightMatrix * entity->GetTransform()).GetData());
			entity->GetMesh()->RenderWithoutMaterial();
		}


		D3DContext::SetRenderTarget(nullptr);

		light->shader->SetTexture(1, light->shadowMap);

		for (uint_t i = 0; i < numEntities; i++) {
			Entity3D* entity = entities[i];
			light->shader->SetVSConstantBuffer("LightMatrix", lightMatrix.GetData());
			light->shader->SetPSConstantBuffer("Light", light->light);
			light->shader->SetVSConstantBuffer("Model", entity->GetTransform().GetData());
			entity->GetMesh()->Render(light->shader);
		}
	} else {
		light->shader->SetTexture(1, nullptr);

		for (uint_t i = 0; i < numEntities; i++) {
			Entity3D* entity = entities[i];
			light->shader->SetPSConstantBuffer("Light", light->light);
			light->shader->SetVSConstantBuffer("Model", entity->GetTransform().GetData());
			entity->GetMesh()->Render(light->shader);
		}
	}

	//Debug::ShowTexture(shadowMap);

	for (uint_t i = 1; i < numLights; i++) {
		SR_Light* light = lights[i];

		if (light->light->GetLightType() & FD_LIGHT_CAST_SHADOW) {
			SetDepth(FD_RENDERER_DEPTH_DEFAULT);
			SetBlend(FD_RENDERER_BLEND_DEFAULT);
			shadowShader->Bind();
			light->shadowMap->BindAsRenderTarget();
			D3DContext::Clear();

			mat4 lightMatrix = light->GetLightMatrix();
			for (uint_t i = 0; i < numEntities; i++) {
				Entity3D* entity = entities[i];
				shadowShader->SetVSConstantBuffer("MVP", (lightMatrix * entity->GetTransform()).GetData());
				entity->GetMesh()->RenderWithoutMaterial();
			}


			D3DContext::SetRenderTarget(nullptr);
	
			light->shader->SetTexture(1, light->shadowMap);

			SetDepth(FD_RENDERER_DEPTH_EQUAL);
			SetBlend(FD_RENDERER_BLEND_ENABLED);

			for (uint_t i = 0; i < numEntities; i++) {
				Entity3D* entity = entities[i];
				light->shader->SetVSConstantBuffer("LightMatrix", lightMatrix.GetData());
				light->shader->SetPSConstantBuffer("Light", light->light);
				light->shader->SetVSConstantBuffer("Model", entity->GetTransform().GetData());
				entity->GetMesh()->Render(light->shader);
			}
		} else {
			SetDepth(FD_RENDERER_DEPTH_EQUAL);
			SetBlend(FD_RENDERER_BLEND_ENABLED);
			light->shader->SetTexture(1, nullptr);

			for (uint_t i = 0; i < numEntities; i++) {
				Entity3D* entity = entities[i];
				light->shader->SetPSConstantBuffer("Light", light->light);
				light->shader->SetVSConstantBuffer("Model", entity->GetTransform().GetData());
				entity->GetMesh()->Render(light->shader);
			}
		}
	}

	SetDepth(FD_RENDERER_DEPTH_DEFAULT);
	SetBlend(FD_RENDERER_BLEND_DEFAULT);
}


mat4 SimpleRenderer::SR_DirectionalLight::GetLightMatrix() const {
	DirectionalLight* l = (DirectionalLight*)light;

	return projection * mat4::LookAt(-l->GetDirection(), vec3(0, 0, 0), vec3(0, 1, 0));
}

mat4 SimpleRenderer::SR_PointLight::GetLightMatrix() const {
	PointLight* l = (PointLight*)light;
	return mat4::Identity();
}

}