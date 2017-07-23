#include "pbrrenderer.h"

namespace FD {

void PBRStaticRenderer::Submit(const RenderCommand& cmd) {
	commandQueue.Push_back(cmd);
}

void PBRStaticRenderer::Submit(Entity3D* e) {
	Submit(e->GetMesh(), e->GetTransform());
}

void PBRStaticRenderer::Submit(Mesh* mesh, mat4 transform) {
	RenderCommand cmd;
	cmd.mesh = mesh;
	cmd.shader = mesh->GetMaterial()->GetShader();
	cmd.transform = transform;
	Submit(cmd);
}

}