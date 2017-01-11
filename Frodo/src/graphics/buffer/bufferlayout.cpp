#include "bufferlayout.h"
#include <graphics/shader/shader.h>

static uint32 get_size_from_format(DXGI_FORMAT format) {
	switch (format) {
		case DXGI_FORMAT_R32_FLOAT: return sizeof(float32);
		case DXGI_FORMAT_R32G32_FLOAT: return sizeof(vec2);
		case DXGI_FORMAT_R32G32B32_FLOAT: return sizeof(vec3);
		case DXGI_FORMAT_R32G32B32A32_FLOAT: return sizeof(vec4);
		case DXGI_FORMAT_R8G8B8A8_UINT:
		case DXGI_FORMAT_R8G8B8A8_SINT:
		case DXGI_FORMAT_R8G8B8A8_UNORM: return sizeof(int32);
		default:
			FD_ASSERT(false && "UNKNOWN FORMAT");
	}

	return 0;
}

BufferLayout::~BufferLayout() {
	
}

void BufferLayout::Push(const String& name, DXGI_FORMAT format, uint32 slot) {
	uint32 size = get_size_from_format(format);
	FD_ASSERT(size);
	elements.Push_back({name, format, slot, size, offset});
	offset += size;
}

void BufferLayout::CreateInputLayout(Shader* shader) {
	D3D11_INPUT_ELEMENT_DESC* desc = new D3D11_INPUT_ELEMENT_DESC[elements.GetSize()];

	uint32 stepRate = 0;
	D3D11_INPUT_CLASSIFICATION input;

	for (uint_t i = 0; i < elements.GetSize(); i++) {
		BufferLayoutAttrib& a = elements[i];

		if (a.slot > 0) {
			stepRate = 1;
			input = D3D11_INPUT_PER_INSTANCE_DATA;
		} else {
			stepRate = 0;
			input = D3D11_INPUT_PER_VERTEX_DATA;
		}

		desc[i] = {*a.name, 0, a.format, a.slot, a.offset, input, stepRate};
	}

	ID3D11InputLayout* tmp = nullptr;
	D3DContext::GetDevice()->CreateInputLayout(desc, (uint32)elements.GetSize(), shader->GetVSBufferPointer(), shader->GetVSBufferSize(), &tmp);

	shader->SetInputLayout(tmp);

	delete[] desc;
}
