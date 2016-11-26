#include "batchrenderer.h"

#include <graphics/shader/shaderfactory.h>


void BatchRenderer::SetBlendingInternal(bool enable_blending) {
	float factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	D3DContext::GetDeviceContext()->OMSetBlendState(blendState[enable_blending ? 1 : 0], factor, 0xFFFFFFFF);
}

void BatchRenderer::SetDepthInternal(bool enable_depthtesting) {
	D3DContext::GetDeviceContext()->OMSetDepthStencilState(depthState[enable_depthtesting ? 1 : 0], 0);
}

void BatchRenderer::CreateBlendStates() {

	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));


	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3DContext::GetDevice()->CreateBlendState(&desc, &blendState[0]);

	// ENABLED

	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3DContext::GetDevice()->CreateBlendState(&desc, &blendState[1]);
}


void BatchRenderer::CreateDepthStates() {


	//DEFAULT
	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	desc.DepthEnable = false;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	desc.StencilEnable = false;

	D3DContext::GetDevice()->CreateDepthStencilState(&desc, &depthState[1]);

	ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));


	//NO DEPTH

	ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_ALWAYS;

	desc.StencilEnable = false;

	D3DContext::GetDevice()->CreateDepthStencilState(&desc, &depthState[0]);
}


void BatchRenderer::Begin() {
	indexCount = 0;
	tids.Clear();
	buffer = (Vertex*)vbo->Map(FD_MAP_WRITE_DISCARD);
}

void BatchRenderer::End() {
	vbo->Unmap();
	buffer = nullptr;
}

float BatchRenderer::SubmitTexture(Texture2D* texture) {
	size_t numTids = tids.GetSize();

	float tid = 0;

	if (tids.Find(texture) == (size_t)-1) {
		if (tids.GetSize() == maxSimultaneousTextures) {
			End();
			Render();
			Begin();

			numTids = 0;
		}

		tids.Push_back(texture);
		tid = (float)numTids + 1.0f;

	}
	else {
		tid = (float)tids.Find(texture) + 1.0f;
	}

	return tid;
}

BatchRenderer::BatchRenderer(Window* window, unsigned int max_vertices, unsigned int max_simultaneous_textures) : Renderer(window, nullptr) {
	this->maxVertices = max_vertices;
	this->indexCount = 0;
	this->maxSimultaneousTextures = max_simultaneous_textures;
	this->buffer = nullptr;
}

BatchRenderer::BatchRenderer(Window* window, Camera* camera, unsigned int max_vertices) : BatchRenderer(window, max_vertices, 16) {
	SetCamera(camera);
}

BatchRenderer::~BatchRenderer() {
	delete ibo;
	delete vbo;
	delete shader;

	DX_FREE(blendState[0]);
	DX_FREE(blendState[1]);
	DX_FREE(depthState[0]);
	DX_FREE(depthState[1]);
}

void BatchRenderer::Submit(Entity* e) {
	FD_WARNING("\"%s\" not implemented", __FUNCSIG__);
}

void BatchRenderer::Render() {
	SetBlendingInternal(blending);
	SetDepthInternal(depthTesting);

	shader->Bind();

	size_t num = tids.GetSize();
	for (size_t i = 0; i < num; i++) {
		shader->SetTexture((unsigned int)i, tids[i]);
	}

	vbo->Bind();
	ibo->Bind();

	D3DContext::GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
}