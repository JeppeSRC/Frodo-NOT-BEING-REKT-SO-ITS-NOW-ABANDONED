#include "material.h"
#include <core/log.h>

namespace FD {

Material::Material(Shader* shader) : shader(shader) {
	vCBuffer.data = nullptr;
	pCBuffer.data = nullptr;

	parent = nullptr;
}

Material::Material(Material* mat) {
	shader = mat->shader;

	vCBuffer = mat->GetVCBuffer();
	vCBuffer.data = vCBuffer.structSize == 0 ? nullptr : new byte[vCBuffer.structSize];
	memcpy(vCBuffer.data, mat->GetVCBuffer().data, vCBuffer.structSize);

	pCBuffer = mat->GetPCBuffer();
	pCBuffer.data = pCBuffer.structSize == 0 ? nullptr : new byte[pCBuffer.structSize];
	memcpy(pCBuffer.data, mat->GetPCBuffer().data, pCBuffer.structSize);

	textures = mat->textures;

	parent = (Material*)mat;
}

Material::Material(const Material& mat) {
	shader = mat.shader;

	vCBuffer = mat.GetVCBuffer();
	vCBuffer.data = vCBuffer.structSize == 0 ? nullptr : new byte[vCBuffer.structSize];
	memcpy(vCBuffer.data, mat.GetVCBuffer().data, vCBuffer.structSize);

	pCBuffer = mat.GetPCBuffer();
	pCBuffer.data = pCBuffer.structSize == 0 ? nullptr : new byte[pCBuffer.structSize];
	memcpy(pCBuffer.data, mat.GetPCBuffer().data, pCBuffer.structSize);

	textures = mat.textures;

	parent = nullptr;
}


Material::~Material() {
	delete vCBuffer.data;
	delete pCBuffer.data;
}

void Material::Bind() {
	Bind(shader);
}

void Material::Bind(Shader* shader) {
	if (vCBuffer.data) shader->SetVSConstantBuffer(vCBuffer);
	if (pCBuffer.data) shader->SetPSConstantBuffer(pCBuffer);

	uint_t size = textures.GetKeyList().GetSize();

	for (uint_t i = 0; i < size; i++) {
		FD_MAP_PAIR<uint32, Texture*> pair = textures.GetPair(i);
		shader->SetTexture(pair.key, pair.data);
	}

	shader->Bind();
}

ID3D11ShaderResourceView* views[128];

void Material::UnBindTextures() {
	D3DContext::GetDeviceContext()->PSSetShaderResources(0, textures.GetItems(), views);
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

	if (data) memcpy(vCBuffer.data, data, toCopy);
	else memset(vCBuffer.data, 0, toCopy);
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

	if (data) memcpy(pCBuffer.data, data, toCopy);
	else memset(pCBuffer.data, 0, toCopy);
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