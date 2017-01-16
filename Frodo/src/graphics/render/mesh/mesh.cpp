#include "mesh.h"

void Mesh::Render() {
	material->Bind();

	vBuffer->Bind();
	iBuffer->Bind();
	D3DContext::GetDeviceContext()->DrawIndexed(iBuffer->GetCount(), 0, 0);
	
}