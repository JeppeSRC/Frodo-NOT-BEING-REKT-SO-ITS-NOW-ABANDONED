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

BufferLayout::~BufferLayout() {
	
}

void BufferLayout::Push(const String& name, DXGI_FORMAT format) {
	unsigned int size = get_size_from_format(format);
	FD_ASSERT(size);
	elements.Push_back({name, format, size, offset});
	offset += size;
}

void BufferLayout::CreateInputLayout(Shader* shader) {
	D3D11_INPUT_ELEMENT_DESC* desc = new D3D11_INPUT_ELEMENT_DESC[elements.GetSize()];

	for (size_t i = 0; i < elements.GetSize(); i++) {
		BufferLayoutAttrib& a = elements[i];
		desc[i] = {*a.name, 0, a.format, 0, a.offset,D3D11_INPUT_PER_VERTEX_DATA, 0};
	}

	ID3D11InputLayout* tmp = nullptr;
	D3DContext::GetDevice()->CreateInputLayout(desc, elements.GetSize(), shader->GetVSBufferPointer(), shader->GetVSBufferSize(), &tmp);

	shader->SetInputLayout(tmp);

	delete[] desc;
}
