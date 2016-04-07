#include "bufferlayout.h"
#include <graphics/shader/shader.h>

static unsigned int get_size_from_format(DXGI_FORMAT format) {
	switch (format) {
		case DXGI_FORMAT_R32G32_FLOAT: return sizeof(vec2);
		case DXGI_FORMAT_R32G32B32_FLOAT: return sizeof(vec3);
		case DXGI_FORMAT_R32G32B32A32_FLOAT: return sizeof(vec4);
		default:
			FD_ASSERT(false && "UNKNOWN FORMAT");
	}

	return 0;
}

FDBufferLayout::~FDBufferLayout() {
	DX_FREE(layout);
}

void FDBufferLayout::Push(const char* name, DXGI_FORMAT format) {
	unsigned int size = get_size_from_format(format);
	FD_ASSERT(size);
	elements.Push_back({String(name), format, size, offset});
	offset += size;
}

void FDBufferLayout::CreateInputLayout(FDShader* shader) {
	D3D11_INPUT_ELEMENT_DESC* desc = new D3D11_INPUT_ELEMENT_DESC[elements.GetSize()];

	for (size_t i = 0; i < elements.GetSize(); i++) {
		BufferLayoutAttrib& a = elements[i];
		desc[i] = {*a.name, 0, a.format, 0, a.offset,D3D11_INPUT_PER_VERTEX_DATA, 0};
	}

	D3DContext::GetDevice()->CreateInputLayout(desc, elements.GetSize(), shader->GetVSBufferPointer(), shader->GetVSBufferSize(), &layout);

	delete[] desc;
}

void FDBufferLayout::Bind() {
	D3DContext::GetDeviceContext()->IASetInputLayout(layout);
}