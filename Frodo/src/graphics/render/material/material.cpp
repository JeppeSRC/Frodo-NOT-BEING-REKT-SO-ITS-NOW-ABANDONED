#include "material.h"
#include <core/log.h>

namespace FD {

Material::Material(Shader* shader) : shader(shader) {
	vCBuffer.data = nullptr;
	pCBuffer.data = nullptr;
}

Material::Material(const Material* mat) {
	shader = mat->shader;

	vCBuffer.data = nullptr;
	pCBuffer.data = nullptr;

}

Material::Material(const Material& mat) {
	shader = mat.shader;

	vCBuffer.data = nullptr;
	pCBuffer.data = nullptr;

}

Material::~Material() {
	delete vCBuffer.data;
	delete pCBuffer.data;
}

void Material::Bind() {
	shader->SetVSConstantBuffer(vCBuffer);
	shader->SetPSConstantBuffer(pCBuffer);

	uint_t size = textures.GetKeyList().GetSize();

	for (uint_t i = 0; i < size; i++) {
		FD_MAP_PAIR<uint32, Texture*> pair = textures.GetPair(i);
		shader->SetTexture(pair.key, pair.data);
	}

	shader->Bind();
}

void Material::SetTexture(const String& name, Texture* texture) {
	textures[shader->GetTextureInfo(name).semRegister] = texture;
}

void Material::SetVCBuffer(const String& name, void* data) {
	delete[] vCBuffer.data;
	vCBuffer = shader->GetVSConstantBufferInfo(name);

	vCBuffer.data = new byte[vCBuffer.structSize];

	uint32 size = vCBuffer.structSize;
	uint32 toCopy = size;

	if (size != vCBuffer.structSize) {
		if (vCBuffer.structSize > size) {
			FD_WARNING("[Material] Not all of the buffer will be set! Constant Buffer \"%s\" is size %u, supplied size is %u.", *name, vCBuffer.structSize, size);
		} else {
			toCopy = vCBuffer.structSize;
			FD_WARNING("[Material] Truncating data! Constant Buffer \"%s\" is size %u, supplied size is %u.", *name, vCBuffer.structSize, size);
		}
	}

	memcpy(vCBuffer.data, data, toCopy);
}

void Material::SetPCBuffer(const String& name, void* data) {
	delete[] vCBuffer.data;
	pCBuffer = shader->GetPSConstantBufferInfo(name);

	pCBuffer.data = new byte[pCBuffer.structSize];

	uint32 size = pCBuffer.structSize;
	uint32 toCopy = size;

	if (size != pCBuffer.structSize) {
		if (pCBuffer.structSize > size) {
			FD_WARNING("[Material] Not all of the buffer will be set! Constant Buffer \"%s\" is size %u, supplied size is %u.", *name, pCBuffer.structSize, size);
		} else {
			toCopy = pCBuffer.structSize;
			FD_WARNING("[Material] Truncating data! Constant Buffer \"%s\" is size %u, supplied size is %u.", *name, pCBuffer.structSize, size);
		}
	}

	memcpy(pCBuffer.data, data, toCopy);
}

void Material::SetVCBufferElement(const String& name, void* data) {
	vCBuffer.SetElement(name, data);
}

void Material::SetPCBufferElement(const String& name, void* data) {
	pCBuffer.SetElement(name, data);
}

void Material::SetVCBufferElement(uint32 index, void* data) {
	vCBuffer.SetElement(index, data);
}

void Material::SetPCBufferElement(uint32 index, void* data) {
	pCBuffer.SetElement(index, data);
}

}