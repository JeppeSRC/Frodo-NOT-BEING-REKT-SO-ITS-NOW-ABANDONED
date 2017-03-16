#include "shader.h"
#include <d3dcompiler.h>
#include <core/log.h>
#include <util/vfs/vfs.h>
#include <math/math.h>

namespace FD {

String Shader::GetFunctionTypeString(FD_SHADER_GEN_FUNCTION_TYPE type) {
	switch (type) {
		case FD_TRUE:
			return "true";
		case FD_FALSE:
			return "false";
		case FD_DEFINED:
			return "defined";
		case FD_EQ:
			return "eq";
		case FD_NEQ:
			return "neq";
		case FD_GR:
			return "gr";
		case FD_LS:
			return "ls";
		case FD_GE:
			return "ge";
		case FD_LE:
			return "le";
	}

	FD_WARNING("[Shader] Unknown function type %d", (int32)type);

	return "UNKNOWN";
}

void Shader::CreateBuffers() {

	for (uint_t i = 0; i < vCBuffers.GetSize(); i++) {
		ShaderStructInfo* cbuffer = vCBuffers[i];

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = cbuffer->structSize;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.StructureByteStride = cbuffer->structSize;
		desc.Usage = D3D11_USAGE_DYNAMIC;

		D3DContext::GetDevice()->CreateBuffer(&desc, 0, &cbuffer->buffer);

	}

	for (uint_t i = 0; i < pCBuffers.GetSize(); i++) {
		ShaderStructInfo* cbuffer = pCBuffers[i];

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = cbuffer->structSize;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.StructureByteStride = cbuffer->structSize;
		desc.Usage = D3D11_USAGE_DYNAMIC;

		D3DContext::GetDevice()->CreateBuffer(&desc, 0, &cbuffer->buffer);

	}
}

void Shader::Compile(String vSource, String pSource) {
	inputLayout = nullptr;
	vByteCode = nullptr;
	pByteCode = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;

	ID3DBlob* error = nullptr;

	D3DCompile(*vSource, vSource.length, 0, 0, 0, "vsMain", "vs_5_0", 0, 0, &vByteCode, &error);

	if (error) {
		FD_FATAL("VertexShader ERROR: %s", error->GetBufferPointer());
		DX_FREE(error);
		FD_ASSERT(vByteCode && "VertexShader failed to compile");
	}

	DX_FREE(error);

	D3DCompile(*pSource, pSource.length, 0, 0, 0, "psMain", "ps_5_0", 0, 0, &pByteCode, &error);

	if (error) {
		FD_FATAL("PixelShader ERROR: %s", error->GetBufferPointer());
		DX_FREE(error);
		FD_ASSERT(pByteCode && "PixelShader failed to compile");
	}

	DX_FREE(error);


	D3DContext::GetDevice()->CreateVertexShader(vByteCode->GetBufferPointer(), vByteCode->GetBufferSize(), 0, &vertexShader);

	FD_ASSERT(vertexShader && "Failed to create vertexshader");

	D3DContext::GetDevice()->CreatePixelShader(pByteCode->GetBufferPointer(), pByteCode->GetBufferSize(), 0, &pixelShader);

	FD_ASSERT(pixelShader && "Failed to create pixelshader");

	ParseStructs(vSource, FD_SHADER_TYPE_VERTEXSHADER);
	ParseStructs(pSource, FD_SHADER_TYPE_PIXELSHADER);

	ParseTextures(pSource);

	CreateBuffers();
}

Shader::Shader(const String& vertexFilename, const String& pixelFilename, bool src) {
	if (src) {
		vSource = vertexFilename;
		pSource = pixelFilename;
	} else {
		vSource = VFS::Get()->ReadTextFile(vertexFilename);
		pSource = VFS::Get()->ReadTextFile(pixelFilename);
	}

	vSourceOriginal = vSource;
	pSourceOriginal = pSource;

	RemoveComments(vSource);
	RemoveComments(pSource);

	ShaderGenParseDefinitions(vSource, FD_SHADER_TYPE_VERTEXSHADER);
	ShaderGenParseDefinitions(pSource, FD_SHADER_TYPE_PIXELSHADER);
}

Shader::~Shader() {
	DX_FREE(inputLayout);
	DX_FREE(vertexShader);
	DX_FREE(pixelShader);
	DX_FREE(vByteCode);
	DX_FREE(pByteCode);

	vCBuffers.Free();
	pCBuffers.Free();
	pTextures.Free();

	variables.Free();
	blocks.Free();
}

void Shader::Bind() {
	D3DContext::GetDeviceContext()->IASetInputLayout(inputLayout);

	D3DContext::GetDeviceContext()->VSSetShader(vertexShader, 0, 0);
	D3DContext::GetDeviceContext()->PSSetShader(pixelShader, 0, 0);


	for (uint_t i = 0; i < vCBuffers.GetSize(); i++) {
		ShaderStructInfo& cb = *vCBuffers[i];
		D3DContext::GetDeviceContext()->VSSetConstantBuffers(cb.semRegister, 1, &cb.buffer);
	}

	for (uint_t i = 0; i < pCBuffers.GetSize(); i++) {
		ShaderStructInfo& cb = *pCBuffers[i];
		D3DContext::GetDeviceContext()->PSSetConstantBuffers(cb.semRegister, 1, &cb.buffer);
	}
}

void Shader::SetVSConstantBufferSlotInternal(uint32 slot, void* data) {
	ShaderStructInfo& cb = *vCBuffers[slot];
	D3D11_MAPPED_SUBRESOURCE sub;
	ZeroMemory(&sub, sizeof(D3D11_MAPPED_SUBRESOURCE));

	D3DContext::GetDeviceContext()->Map((ID3D11Resource*)cb.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
	memcpy(sub.pData, data, cb.structSize);
	D3DContext::GetDeviceContext()->Unmap((ID3D11Resource*)cb.buffer, 0);
}

void Shader::SetPSConstantBufferSlotInternal(uint32 slot, void* data) {
	ShaderStructInfo& cb = *pCBuffers[slot];
	D3D11_MAPPED_SUBRESOURCE sub;
	ZeroMemory(&sub, sizeof(D3D11_MAPPED_SUBRESOURCE));

	D3DContext::GetDeviceContext()->Map((ID3D11Resource*)cb.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
	memcpy(sub.pData, data, cb.structSize);
	D3DContext::GetDeviceContext()->Unmap((ID3D11Resource*)cb.buffer, 0);
}

void Shader::SetVSConstantBuffer(const String& bufferName, void* data) {
	uint_t size = vCBuffers.GetSize();
	for (uint_t i = 0; i < size; i++) {
		if (vCBuffers[i]->name == bufferName) SetVSConstantBufferSlotInternal((uint32)i, data);
		return;
	}

	FD_WARNING("[Shader] Buffer not found \"%s\"", *bufferName);
}

void Shader::SetPSConstantBuffer(const String& bufferName, void* data) {
	uint_t size = pCBuffers.GetSize();
	for (uint_t i = 0; i < size; i++) {
		if (pCBuffers[i]->name == bufferName) SetPSConstantBufferSlotInternal((uint32)i, data);
		return;
	}

	FD_WARNING("[Shader] Buffer not found \"%s\"", *bufferName);
}

void Shader::SetVSConstantBuffer(uint32 slot, void* data) {
	uint_t size = vCBuffers.GetSize();
	for (uint_t i = 0; i < size; i++) {
		if (vCBuffers[i]->semRegister == slot) {
			SetVSConstantBufferSlotInternal(slot, data);
			return;
		}
	}

	FD_WARNING("[Shader] No buffer at slot %u", slot);
}

void Shader::SetPSConstantBuffer(uint32 slot, void* data) {
	uint_t size = pCBuffers.GetSize();
	for (uint_t i = 0; i < size; i++) {
		if (pCBuffers[i]->semRegister == slot) {
			SetPSConstantBufferSlotInternal((uint32)i, data);
			return;
		}
	}

	FD_WARNING("[Shader] No buffer at slot %u", slot);
}

void Shader::SetVSConstantBuffer(Shader::ConstantBufferSlot vCBuffer) {
	SetVSConstantBufferSlotInternal(vCBuffer.semRegister, vCBuffer.data);
}

void Shader::SetPSConstantBuffer(Shader::ConstantBufferSlot pCBuffer) {
	SetPSConstantBufferSlotInternal(pCBuffer.semRegister, pCBuffer.data);
}

void Shader::SetTexture(uint32 slot, const Texture* tex) {
	ID3D11ShaderResourceView* view = tex == nullptr ? nullptr : tex->GetResourceView();
	D3DContext::GetDeviceContext()->PSSetShaderResources(slot, 1, &view);
}

Shader::ConstantBufferSlot Shader::GetVSConstantBufferInfo(const String& name) {
	uint_t size = vCBuffers.GetSize();
	for (uint_t i = 0; i < size; i++) {
		ShaderStructInfo* info = vCBuffers[i];
		if (info->name == name) return ConstantBufferSlot(info->semRegister, info->structSize, nullptr, info->layout);
	}

	FD_FATAL("[Shader] No buffer named \"%s\"", *name);
	return ConstantBufferSlot(-1, -1, nullptr, BufferLayout());
}

Shader::ConstantBufferSlot Shader::GetPSConstantBufferInfo(const String& name) {
	uint_t size = pCBuffers.GetSize();
	for (uint_t i = 0; i < size; i++) {
		ShaderStructInfo* info = pCBuffers[i];
		if (info->name == name) return ConstantBufferSlot(info->semRegister, info->structSize, nullptr, info->layout);
	}

	FD_FATAL("[Shader] No buffer named \"%s\"", *name);
	return ConstantBufferSlot(-1, -1, nullptr, BufferLayout());
}

Shader::TextureSlot Shader::GetTextureInfo(const String& name) {
	uint_t size = pTextures.GetSize();
	for (uint_t i = 0; i < size; i++) {
		ShaderTextureInfo* info = pTextures[i];
		if (info->name == name) return TextureSlot(info->semRegister, info->numTextures);
	}

	FD_FATAL("[Shader] No texture named \"%s\"", *name);
	return TextureSlot(-1, -1);
}

uint32 Shader::GetVSConstantBufferSlotByName(const String& bufferName) {
	uint_t size = vCBuffers.GetSize();
	for (uint_t i = 0; i < size; i++) {
		if (vCBuffers[i]->name == bufferName) return vCBuffers[i]->semRegister;
	}

	FD_FATAL("[Shader] Buffer not found \"%s\"", *bufferName);
	return -1;
}

uint32 Shader::GetPSConstantBufferSlotByName(const String& bufferName) {
	uint_t size = pCBuffers.GetSize();
	for (uint_t i = 0; i < size; i++) {
		if (pCBuffers[i]->name == bufferName) return pCBuffers[i]->semRegister;
	}

	FD_FATAL("[Shader] Buffer not found \"%s\"", *bufferName);
	return -1;
}

uint32 Shader::GetPSTextureSlotByName(const String& textureName) {
	uint_t size = pTextures.GetSize();
	for (uint_t i = 0; i < size; i++) {
		if (pTextures[i]->name == textureName) return pTextures[i]->semRegister;
	}

	FD_FATAL("[Shader] Texture not found \"%s\"", *textureName);
	return -1;
}

}