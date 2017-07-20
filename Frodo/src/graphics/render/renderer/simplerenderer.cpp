#include "simplerenderer.h"

namespace FD {

SimpleRenderer::SimpleRenderer(Window* window) : Renderer(window) {
	CreateDepthAndBlendStates();
	InitializeShaders();

	camera = geometryShader->GetPSConstantBufferInfo("Camera");
}

SimpleRenderer::~SimpleRenderer() {

}

void SimpleRenderer::Begin(Camera* camera) {
	this->camera.SetElement("c_Position", (void*)&camera->GetPosition());
	this->camera.SetElement("c_ViewMatrix", (void*)camera->GetViewMatrix().GetData());
	this->camera.SetElement("c_ProjectionMatrix", (void*)camera->GetProjectionMatrix().GetData());
}

void SimpleRenderer::Submit(const LightStack& lightStack) {

}

void SimpleRenderer::Submit(Light* light) {
	if (light->GetLightType() != FD_LIGHT_TYPE_POINT) {
		FD_FATAL("[SimpleRenderer] PointLights only supported atm!");
		return;
	}

	lights.Push_back((PointLight*)light);
}

void SimpleRenderer::End() {

}

void SimpleRenderer::Present() {

}

}