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

	for (uint_t i = 0; i < gCBuffers.GetSize(); i++) {
		ShaderStructInfo* cbuffer = gCBuffers[i];

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

void Shader::Compile(String vSource, String pSource, String gSource, bool geometry) {
	inputLayout = nullptr;
	vByteCode = nullptr;
	pByteCode = nullptr;
	gByteCode = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;
	geometryShader = nullptr;

	ParseStructs(vSource, FD_SHADER_TYPE_VERTEXSHADER);
	ParseStructs(pSource, FD_SHADER_TYPE_PIXELSHADER);
	if (geometry) ParseStructs(gSource, FD_SHADER_TYPE_GEOMETRYSHADER);

	ParseTextures(pSource);

	ID3DBlob* error = nullptr;

	D3DCompile(*vSource, vSource.length, 0, 0, 0, "vsMain", "vs_5_0", 0, 0, &vByteCode, &error);

	if (error) {
		FD_FATAL("VertexShader ERROR: %s", error->GetBufferPointer());
		DX_FREE(error);
		FD_ASSERT_MSG(vByteCode == nullptr, "VertexShader failed to compile");
	}

	DX_FREE(error);

	D3DCompile(*pSource, pSource.length, 0, 0, 0, "psMain", "ps_5_0", 0, 0, &pByteCode, &error);

	if (error) {
		FD_FATAL("PixelShader ERROR: %s", error->GetBufferPointer());
		DX_FREE(error);
		FD_ASSERT_MSG(pByteCode == nullptr, "PixelShader failed to compile");
	}

	DX_FREE(error);

	if (geometry) {
		D3DCompile(*gSource, gSource.length, 0, 0, 0, "gsMain", "gs_5_0", 0, 0, &gByteCode, &error);

		if (error) {
			FD_FATAL("GeometryShader ERROR: %s", error->GetBufferPointer());
			DX_FREE(error);
			FD_ASSERT_MSG(pByteCode == nullptr, "GeometryShader failed to compile");
		}

		DX_FREE(error);

		D3DContext::GetDevice()->CreateGeometryShader(gByteCode->GetBufferPointer(), gByteCode->GetBufferSize(), 0, &geometryShader);

		FD_ASSERT_MSG(geometryShader == nullptr, "Failed to create geometryShader");
	}

	D3DContext::GetDevice()->CreateVertexShader(vByteCode->GetBufferPointer(), vByteCode->GetBufferSize(), 0, &vertexShader);

	FD_ASSERT_MSG(vertexShader == nullptr, "Failed to create vertexshader");

	D3DContext::GetDevice()->CreatePixelShader(pByteCode->GetBufferPointer(), pByteCode->GetBufferSize(), 0, &pixelShader);

	FD_ASSERT_MSG(pixelShader == nullptr, "Failed to create pixelshader");

	CreateBuffers();
}

Shader::Shader(const String& vertexFilename, const String& pixelFilename, const String& geometryFilename, bool src) {
	if (src) {
		vSource = vertexFilename;
		pSource = pixelFilename;
		gSource = geometryFilename;
	} else {
		vSource = VFS::Get()->ReadTextFile(vertexFilename);
		pSource = VFS::Get()->ReadTextFile(pixelFilename);
		gSource = VFS::Get()->ReadTextFile(geometryFilename);
	}

	vSourceOriginal = vSource;
	pSourceOriginal = pSource;
	gSourceOriginal = gSource;

	RemoveComments(vSource);
	RemoveComments(pSource);
	RemoveComments(gSource);

	ShaderGenParseDefinitions(vSource, FD_SHADER_TYPE_VERTEXSHADER);
	ShaderGenParseDefinitions(pSource, FD_SHADER_TYPE_PIXELSHADER);
	ShaderGenParseDefinitions(gSource, FD_SHADER_TYPE_GEOMETRYSHADER);
}

Shader::~Shader() {
	DX_FREE(inputLayout);
	DX_FREE(vertexShader);
	DX_FREE(pixelShader);
	DX_FREE(vByteCode);
	DX_FREE(pByteCode);

	vStructs.Free();
	pStructs.Free();

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
	D3DContext::GetDeviceContext()->GSSetShader(geometryShader, 0, 0);


	for (uint_t i = 0; i < vCBuffers.GetSize(); i++) {
		ShaderStructInfo& cb = *vCBuffers[i];
		D3DContext::GetDeviceContext()->VSSetConstantBuffers(cb.semRegister, 1, &cb.buffer);
	}

	for (uint_t i = 0; i < pCBuffers.GetSize(); i++) {
		ShaderStructInfo& cb = *pCBuffers[i];
		D3DContext::GetDeviceContext()->PSSetConstantBuffers(cb.semRegister, 1, &cb.buffer);
	}

	for (uint_t i = 0; i < gCBuffers.GetSize(); i++) {
		ShaderStructInfo& cb = *gCBuffers[i];
		D3DContext::GetDeviceContext()->GSSetConstantBuffers(cb.semRegister, 1, &cb.buffer);
	}
}

void Shader::SetVSConstantBufferSlotInternal(uint32 slot, const void* data) const{
	ShaderStructInfo& cb = *vCBuffers.Get(slot);
	D3D11_MAPPED_SUBRESOURCE sub;
	ZeroMemory(&sub, sizeof(D3D11_MAPPED_SUBRESOURCE));

	D3DContext::GetDeviceContext()->Map((ID3D11Resource*)cb.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
	memcpy(sub.pData, data, cb.structSize);
	D3DContext::GetDeviceContext()->Unmap((ID3D11Resource*)cb.buffer, 0);
}

void Shader::SetPSConstantBufferSlotInternal(uint32 slot, const void* data) const {
	ShaderStructInfo& cb = *pCBuffers.Get(slot);
	D3D11_MAPPED_SUBRESOURCE sub;
	ZeroMemory(&sub, sizeof(D3D11_MAPPED_SUBRESOURCE));

	D3DContext::GetDeviceContext()->Map((ID3D11Resource*)cb.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
	memcpy(sub.pData, data, cb.structSize);
	D3DContext::GetDeviceContext()->Unmap((ID3D11Resource*)cb.buffer, 0);
}

void Shader::SetGSConstantBufferSlotInternal(uint32 slot, const void* data) const {
	ShaderStructInfo& cb = *gCBuffers.Get(slot);
	D3D11_MAPPED_SUBRESOURCE sub;
	ZeroMemory(&sub, sizeof(D3D11_MAPPED_SUBRESOURCE));

	D3DContext::GetDeviceContext()->Map((ID3D11Resource*)cb.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
	memcpy(sub.pData, data, cb.structSize);
	D3DContext::GetDeviceContext()->Unmap((ID3D11Resource*)cb.buffer, 0);
}

void Shader::SetVSConstantBuffer(const String& bufferName, const void* data) const {
	uint_t size = vCBuffers.GetSize();
	for (uint_t i = 0; i < size; i++) {
		if (vCBuffers.Get(i)->name == bufferName) {
			SetVSConstantBufferSlotInternal((uint32)i, data);
			return;
		}
	}

	FD_WARNING("[Shader] Buffer not found \"%s\"", *bufferName);
}

void Shader::SetPSConstantBuffer(const String& bufferName, const void* data) const {
	uint_t size = pCBuffers.GetSize();
	for (uint_t i = 0; i < size; i++) {
		if (pCBuffers.Get(i)->name == bufferName) {
			SetPSConstantBufferSlotInternal((uint32)i, data);
			return;
		}
	}

	FD_WARNING("[Shader] Buffer not found \"%s\"", *bufferName);
}

void Shader::SetGSConstantBuffer(const String& bufferName, const void* data) const {
	uint_t size = gCBuffers.GetSize();
	for (uint_t i = 0; i < size; i++) {
		if (gCBuffers.Get(i)->name == bufferName) {
			SetGSConstantBufferSlotInternal((uint32)i, data);
			return;
		}
	}

	FD_WARNING("[Shader] Buffer not found \"%s\"", *bufferName);
}

void Shader::SetVSConstantBuffer(uint32 slot, const void* data) const {
	uint_t size = vCBuffers.GetSize();
	for (uint_t i = 0; i < size; i++) {
		if (vCBuffers.Get(i)->semRegister == slot) {
			SetVSConstantBufferSlotInternal((uint32)i, data);
			return;
		}
	}

	FD_WARNING("[Shader] No buffer at slot %u", slot);
}

void Shader::SetPSConstantBuffer(uint32 slot, const void* data) const {
	uint_t size = pCBuffers.GetSize();
	for (uint_t i = 0; i < size; i++) {
		if (pCBuffers.Get(i)->semRegister == slot) {
			SetPSConstantBufferSlotInternal((uint32)i, data);
			return;
		}
	}

	FD_WARNING("[Shader] No buffer at slot %u", slot);
}

void Shader::SetGSConstantBuffer(uint32 slot, const void* data) const {
	uint_t size = gCBuffers.GetSize();
	for (uint_t i = 0; i < size; i++) {
		if (gCBuffers.Get(i)->semRegister == slot) {
			SetGSConstantBufferSlotInternal((uint32)i, data);
			return;
		}
	}

	FD_WARNING("[Shader] No buffer at slot %u", slot);
}

void Shader::SetVSConstantBuffer(const Shader::ConstantBufferSlot vCBuffer) const{
	SetVSConstantBuffer(vCBuffer.semRegister, vCBuffer.data);
}

void Shader::SetPSConstantBuffer(const Shader::ConstantBufferSlot pCBuffer) const {
	SetPSConstantBuffer(pCBuffer.semRegister, pCBuffer.data);
}

void Shader::SetGSConstantBuffer(const Shader::ConstantBufferSlot gCBuffer) const {
	SetGSConstantBuffer(gCBuffer.semRegister, gCBuffer.data);
}

void Shader::SetTexture(uint32 slot, const Texture* tex) const {
	ID3D11ShaderResourceView* view = tex == nullptr ? nullptr : tex->GetResourceView();
	D3DContext::GetDeviceContext()->PSSetShaderResources(slot, 1, &view);
}

Shader::ConstantBufferSlot Shader::GetVSConstantBufferInfo(const String& name) const {
	uint_t size = vCBuffers.GetSize();
	for (uint_t i = 0; i < size; i++) {
		ShaderStructInfo* info = vCBuffers.Get(i);
		if (info->name == name) return ConstantBufferSlot(info->semRegister, info->structSize, nullptr, info->layout);
	}

	FD_FATAL("[Shader] No buffer named \"%s\"", *name);
	return ConstantBufferSlot(-1, -1, nullptr, BufferLayout());
}

Shader::ConstantBufferSlot Shader::GetPSConstantBufferInfo(const String& name) const {
	uint_t size = pCBuffers.GetSize();
	for (uint_t i = 0; i < size; i++) {
		ShaderStructInfo* info = pCBuffers.Get(i);
		if (info->name == name) return ConstantBufferSlot(info->semRegister, info->structSize, nullptr, info->layout);
	}

	FD_FATAL("[Shader] No buffer named \"%s\"", *name);
	return ConstantBufferSlot(-1, -1, nullptr, BufferLayout());
}

Shader::ConstantBufferSlot Shader::GetGSConstantBufferInfo(const String& name) const {
	uint_t size = gCBuffers.GetSize();
	for (uint_t i = 0; i < size; i++) {
		ShaderStructInfo* info = gCBuffers.Get(i);
		if (info->name == name) return ConstantBufferSlot(info->semRegister, info->structSize, nullptr, info->layout);
	}

	FD_FATAL("[Shader] No buffer named \"%s\"", *name);
	return ConstantBufferSlot(-1, -1, nullptr, BufferLayout());
}

Shader::TextureSlot Shader::GetTextureInfo(const String& name) const {
	uint_t size = pTextures.GetSize();
	for (uint_t i = 0; i < size; i++) {
		ShaderTextureInfo* info = pTextures.Get(i);
		if (info->name == name) return TextureSlot(info->semRegister, info->numTextures);
	}

	FD_FATAL("[Shader] No texture named \"%s\"", *name);
	return TextureSlot(-1, -1);
}

uint32 Shader::GetVSConstantBufferSlotByName(const String& bufferName) const {
	uint_t size = vCBuffers.GetSize();
	for (uint_t i = 0; i < size; i++) {
		if (vCBuffers.Get(i)->name == bufferName) return vCBuffers.Get(i)->semRegister;
	}

	FD_FATAL("[Shader] Buffer not found \"%s\"", *bufferName);
	return -1;
}

uint32 Shader::GetPSConstantBufferSlotByName(const String& bufferName) const {
	uint_t size = pCBuffers.GetSize();
	for (uint_t i = 0; i < size; i++) {
		if (pCBuffers.Get(i)->name == bufferName) return pCBuffers.Get(i)->semRegister;
	}

	FD_FATAL("[Shader] Buffer not found \"%s\"", *bufferName);
	return -1;
}

uint32 Shader::GetGSConstantBufferSlotByName(const String& bufferName) const {
	uint_t size = gCBuffers.GetSize();
	for (uint_t i = 0; i < size; i++) {
		if (gCBuffers.Get(i)->name == bufferName) return gCBuffers.Get(i)->semRegister;
	}

	FD_FATAL("[Shader] Buffer not found \"%s\"", *bufferName);
	return -1;
}

uint32 Shader::GetPSTextureSlotByName(const String& textureName) const {
	uint_t size = pTextures.GetSize();
	for (uint_t i = 0; i < size; i++) {
		if (pTextures.Get(i)->name == textureName) return pTextures.Get(i)->semRegister;
	}

	FD_FATAL("[Shader] Texture not found \"%s\"", *textureName);
	return -1;
}

void Shader::ConstantBufferSlot::SetElement(const String& name, const void* data) const {
	uint32 offset = layout.GetElementOffset(name);
	if (offset == (uint32)-1) return;
	memcpy(this->data + offset, data, layout.GetElementSize(name));
}

void Shader::ConstantBufferSlot::SetElement(uint32 index, const void* data) const {
	uint32 offset = layout.GetElementOffset(index);
	if (offset == (uint32)-1) return;
	memcpy(this->data + offset, data, layout.GetElementSize(index));
}

}