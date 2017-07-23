#include "mesh.h"

namespace FD {

void Mesh::Render() {
	if (material) material->Bind();

	vBuffer->Bind();
	iBuffer->Bind();
	D3DContext::GetDeviceContext()->DrawIndexed(iBuffer->GetCount(), 0, 0);

}

void Mesh::Render(Shader* shader) {
	if (material) material->Bind(shader);

	vBuffer->Bind();
	iBuffer->Bind();
	D3DContext::GetDeviceContext()->DrawIndexed(iBuffer->GetCount(), 0, 0);

	if (material) material->UnBindTextures();
}

void Mesh::RenderWithoutMaterial() {
	vBuffer->Bind();
	iBuffer->Bind();
	D3DContext::GetDeviceContext()->DrawIndexed(iBuffer->GetCount(), 0, 0);
}

}