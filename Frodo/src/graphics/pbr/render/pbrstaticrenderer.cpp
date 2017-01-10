#include "pbrrenderer.h"
#include <core/window.h>

PBRStaticRenderer::PBRStaticRenderer(Window* window, Shader* shader, Shader* environmentShader, Camera* camera) {
	SetWindow(window);
	SetShader(shader);
	SetEnvironmentShader(environmentShader);
	SetCamera(camera);

}

PBRStaticRenderer::~PBRStaticRenderer() {

}

void PBRStaticRenderer::Render() {
	environmentShader->Bind();

	mat4 m = camera->GetProjectionMatrix() * camera->GetViewMatrix();

	environmentShader->SetVSConstantBuffer("invViewProjection", &mat4::Inverse(m));

	environment->Bind();
	environment->cubeMap->Bind(0);

	D3DContext::GetDeviceContext()->DrawIndexed(36, 0, 0);
}

void PBRStaticRenderer::Add(Entity* e) {
	entities.Push_back(e);
}

void PBRStaticRenderer::Add(PBRRenderer::FD_PBR_RENDER_CMD cmd) {
	commands.Push_back(cmd);
}


void PBRStaticRenderer::Add(PointLight* light) {
	pointLights.Push_back(light);
}