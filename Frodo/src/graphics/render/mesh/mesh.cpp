#include "mesh.h"

namespace FD {

void Mesh::Render() {
	material->Bind();

	vBuffer->Bind();
	iBuffer->Bind();
	D3DContext::GetDeviceContext()->DrawIndexed(iBuffer->GetCount(), 0, 0);

}

}