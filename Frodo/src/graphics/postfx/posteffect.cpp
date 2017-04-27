#include "posteffect.h"
#include <graphics/d3dcontext.h>
#include <core/window.h>

namespace FD {

PostEffect::PostEffect(Shader* shader, Texture* texture) {
	this->shader = shader;
	this->texture = texture;

	quad = MeshFactory::CreatePlane(vec2(2, 2), nullptr);

	Window* w = D3DContext::GetWindow();

	framebuffer = new Framebuffer2D(w->GetWidth(), w->GetHeight(), FD_TEXTURE_FORMAT_FLOAT_32_32_32_32, false);
}

PostEffect::~PostEffect() {
	delete quad;
	delete framebuffer;
}

void PostEffect::SetPData(const void* data) {
	shader->SetPSConstantBuffer(0, data);
}

void PostEffect::SetVData(const void* data) {
	shader->SetVSConstantBuffer(0, data);
}

Framebuffer2D* PostEffect::Render() {
	framebuffer->BindAsRenderTarget();
	RenderAndDisplay();
	return framebuffer;
}

void PostEffect::RenderAndDisplay() {
	shader->Bind();
	texture->Bind(0);

	quad->Render();
}

}