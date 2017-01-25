#include "pbrrenderer.h"
#include <core/window.h>

struct Ca {
	vec3 pos;
	float pad;
	mat4 view;
	mat4 proj;
};

PBRStaticRenderer::PBRStaticRenderer(Window* window) : Renderer(window) {
	light.semRegister = 0;
	light.structSize = sizeof(PointLight);
	light.data = new byte[light.structSize];

	BufferLayout& l = camera.layout;
	l.PushElement("c_Position", sizeof(vec3));
	l.PushElement("c_Pad0", sizeof(float));
	l.PushElement("c_ViewMatrix", sizeof(mat4));
	l.PushElement("c_ProjectionMatrix", sizeof(mat4));
	
	camera.semRegister = 1;
	camera.structSize = l.GetSize();
	camera.data = new byte[camera.structSize];
	
}

PBRStaticRenderer::~PBRStaticRenderer() {
	delete light.data;
	delete camera.data;
}

void PBRStaticRenderer::Begin(Camera* cam) {
	commandQueue.Clear();

	camera.SetElement("c_Position", (void*)&cam->GetPosition());
	camera.SetElement("c_ViewMatrix", (void*)cam->GetViewMatrix().GetData());
	camera.SetElement("c_ProjectionMatrix", (void*)cam->GetProjectionMatrix().GetData());
}

void PBRStaticRenderer::Submit(const LightStack& lights) {
	Light* l = lights.GetLights()[0];
	if (l->GetLightType() != FD_LIGHT_TYPE_POINT) {
		FD_WARNING("PBR only supports point lights!");
		return;
	}

	memcpy(light.data, l, sizeof(PointLight));
}

void PBRStaticRenderer::End() {

}

void PBRStaticRenderer::Present() {
	
	uint_t size = commandQueue.GetSize();
	for (uint_t i = 0; i < size; i++) {
		const RenderCommand& cmd = commandQueue[i];
		cmd.shader->SetVSConstantBuffer(camera);
		cmd.shader->SetPSConstantBuffer(light);
		cmd.shader->SetVSConstantBuffer("Model", (void*)cmd.transform.GetData());
		cmd.mesh->Render();
	}
}