#include "simplerenderer.h"
#include <graphics/render/mesh/meshfactory.h>

static const char* gVertexShader =
#include <graphics/shader/shaders/deferred/gVertex.hlsl>
;

static const char* gPixelShader =
#include <graphics/shader/shaders/deferred/gPixel.hlsl>
;

static const char* lVertexShader =
#include <graphics/shader/shaders/deferred/lVertex.hlsl>
;

static const char* pPixelShader =
#include <graphics/shader/shaders/deferred/pPixel.hlsl>
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
	geometryShader = new Shader(gVertexShader, gPixelShader, true);
	geometryShader->ShaderGenComplete();

	pointShader = new Shader(lVertexShader, pPixelShader, true);
	pointShader->ShaderGenComplete();

	BufferLayout gLayout;

	gLayout.Push<vec3>("POSITION");
	gLayout.Push<vec3>("NORMAL");
	gLayout.Push<vec2>("TEXCOORD");

	gLayout.CreateInputLayout(geometryShader);
	gLayout.CreateInputLayout(pointShader);
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

	camera = geometryShader->GetVSConstantBufferInfo("Camera");
	camera.data = new byte[camera.structSize];

	renderTarget.Init(window->GetWidth(), window->GetHeight(), FD_TEXTURE_FORMAT_FLOAT_32_32_32_32);

	MaterialInstance* matInst = new MaterialInstance(pointShader);

	matInst->SetTexture("positions", renderTarget[0]);
	matInst->SetTexture("diffuse", renderTarget[1]);
	matInst->SetTexture("normals", renderTarget[2]);
	
	matInst->SetPCBuffer("Light", nullptr);

	plane = MeshFactory::CreatePlane(2, 2, matInst);
}

SimpleRenderer::~SimpleRenderer() {
	delete geometryShader;
	delete pointShader;
	delete plane;

	DX_FREE(depthState[0]);
	DX_FREE(depthState[1]);
	DX_FREE(blendState[0]);
	DX_FREE(blendState[1]);
}

void SimpleRenderer::Begin(Camera* camera) {
//	lights.Clear();
	commandQueue.Clear();
	this->camera.SetElement("c_Position", (void*)&camera->GetPosition());
	this->camera.SetElement("c_ViewMatrix", (void*)camera->GetViewMatrix().GetData());
	this->camera.SetElement("c_ProjectionMatrix", (void*)camera->GetProjectionMatrix().GetData());
}

void SimpleRenderer::Submit(Light* light) {
	if (light->GetLightType() != FD_LIGHT_TYPE_POINT) {
		FD_FATAL("[SimpleRenderer] Only PointLights supported atm!");
		return;
	}

	lights.Push_back((PointLight*)light);
}

void SimpleRenderer::End() {

}

void SimpleRenderer::Present() {
	uint_t numLights = lights.GetSize();
	if (numLights == 0) return;
	SetDepth(FD_RENDERER_DEPTH_DEFAULT);
	SetBlend(FD_RENDERER_BLEND_DEFAULT);

	geometryShader->SetVSConstantBuffer(camera);

	renderTarget.BindAsRenderTarget();
	D3DContext::Clear(3);

	uint_t numCommands = commandQueue.GetSize();

	for (uint_t i = 0; i < numCommands; i++) {
		RenderCommand& cmd = commandQueue[i];
		geometryShader->SetVSConstantBuffer("Model", cmd.transform.GetData());
		cmd.mesh->Render();
	}

	D3DContext::SetRenderTarget(nullptr);

	MaterialInstance* mat = plane->GetMaterial();
	mat->SetPCBufferElement("light", (void*)lights[0]);
	
	plane->Render();

	SetDepth(FD_RENDERER_DEPTH_EQUAL);
	SetBlend(FD_RENDERER_BLEND_ENABLED);

	for (uint_t i = 1; i < numLights; i++) {
		mat->SetPCBufferElement("light", (void*)lights[i]);
		plane->Render();
	}

	ID3D11ShaderResourceView* v[3]{ nullptr, nullptr, nullptr };

	D3DContext::GetDeviceContext()->PSSetShaderResources(0, 3, v);
	
	SetDepth(FD_RENDERER_DEPTH_DEFAULT);
	SetBlend(FD_RENDERER_BLEND_DEFAULT);
}

}