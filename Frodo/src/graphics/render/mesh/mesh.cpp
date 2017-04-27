#include "mesh.h"

namespace FD {

void Mesh::Render() {
	if (material) material->Bind();

	vBuffer->Bind();
	iBuffer->Bind();
	D3DContext::GetDeviceContext()->DrawIndexed(iBuffer->GetCount(), 0, 0);

}

}