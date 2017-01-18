#include "renderer.h"

void Renderer::Submit(const RenderCommand& cmd) {
	commandQueue.Push_back(cmd);
}

void Renderer::Submit(Entity* e) {
	Submit(e->GetMesh(), e->GetTransform());
}

void Renderer::Submit(Mesh* mesh, mat4 transform) {
	RenderCommand cmd;
	cmd.mesh = mesh;
	cmd.shader = mesh->GetMaterial()->GetShader();
	cmd.transform = transform;
	Submit(cmd);
}