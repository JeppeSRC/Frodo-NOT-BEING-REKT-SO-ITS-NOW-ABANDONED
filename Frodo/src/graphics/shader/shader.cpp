#include "shader.h"
#include <d3dcompiler.h>
#include <core/log.h>
#include <util/vfs/vfs.h>
#include <math/math.h>

inline static String get_field_type_as_string(FD_SHADER_FIELD_TYPE type) {
	switch (type) {
		case FD_SHADER_FIELD_TYPE_UNKOWN: return ("UNKOWN");
		case FD_SHADER_FIELD_TYPE_MAT4: return ("float4x4");
		case FD_SHADER_FIELD_TYPE_MAT3: return ("float3x3");
		case FD_SHADER_FIELD_TYPE_VEC4: return ("float4");
		case FD_SHADER_FIELD_TYPE_VEC3: return ("float3");
		case FD_SHADER_FIELD_TYPE_VEC2: return ("float2");
		case FD_SHADER_FIELD_TYPE_FLOAT: return ("float");
	}
	return ("ERROR");
}

inline static unsigned int get_field_type_size(FD_SHADER_FIELD_TYPE type) {
	switch (type) {
		case FD_SHADER_FIELD_TYPE_UNKOWN: return 0;
		case FD_SHADER_FIELD_TYPE_MAT4: return sizeof(mat4);
		case FD_SHADER_FIELD_TYPE_MAT3: return sizeof(mat3);
		case FD_SHADER_FIELD_TYPE_VEC4: return sizeof(vec4);
		case FD_SHADER_FIELD_TYPE_VEC3: return sizeof(vec3);
		case FD_SHADER_FIELD_TYPE_VEC2: return sizeof(vec2);
		case FD_SHADER_FIELD_TYPE_FLOAT: return sizeof(float);
	}

	return 0;
}

void Shader::RemoveComments(String& source) {

	while (true) {
		size_t start = source.Find("/*");

		if (start == -1) break;

		size_t end = source.Find("*/", start);

		source.Remove(start, end+2);
	}

	while (true) {
		size_t start = source.Find("//");

		if (start == -1) break;

		size_t end = source.Find("\n", start);
		if (end == -1) end = source.length;

		source.Remove(start, end);
	}

}

void Shader::ParseStructs(String source, FD_SHADER_TYPE type) {

	while(true) {
		size_t cbufferStart = source.Find("cbuffer") + 7;

		if (cbufferStart < 7) break;

		size_t colon = source.Find(":", cbufferStart);

		String name(source.str + cbufferStart, colon - cbufferStart);
		name.RemoveBlankspace();

		size_t regIndex = source.Find("register", colon) + 10;

		ShaderStructInfo* cbuffer = new ShaderStructInfo;

		cbuffer->name = name;
		cbuffer->semRegister = atoi(*source + regIndex);

		CalcStructSize(source, cbufferStart-7, cbuffer);
		
		switch (type) {
			case FD_SHADER_TYPE_VERTEXSHADER:
				vCBuffers.Push_back(cbuffer);
				break;
			case FD_SHADER_TYPE_PIXELSHADER:
				pCBuffers.Push_back(cbuffer);
				break;
		}
	}

	
}

void Shader::CalcStructSize(String& structSource, size_t offset, ShaderStructInfo* cbuffer) {

	FD_SHADER_FIELD_TYPE types[6]{
		FD_SHADER_FIELD_TYPE_MAT4,
		FD_SHADER_FIELD_TYPE_MAT3,
		FD_SHADER_FIELD_TYPE_VEC4,
		FD_SHADER_FIELD_TYPE_VEC3,
		FD_SHADER_FIELD_TYPE_VEC2,
		FD_SHADER_FIELD_TYPE_FLOAT};

	size_t end = structSource.Find("};", offset);

	String fields(*structSource + offset, end - offset);

	size_t numFields = fields.Count(";");

	size_t currSemicolon = fields.Find(";");
	size_t fieldOffset = 0;

	cbuffer->structSize = 0;

	for (size_t num = 0; num < numFields; num++) {
		for (size_t i = 0; i < 6; i++) {
			size_t index = fields.Find(get_field_type_as_string(types[i]), fieldOffset);

			if (index > currSemicolon || index == -1) continue;

			cbuffer->structSize += get_field_type_size(types[i]);
			
			fieldOffset = currSemicolon;
			currSemicolon = fields.Find(";", currSemicolon + 1);

			i = 6;
		}
	}

	structSource.Remove(offset, end+2);
}

void Shader::CreateBuffers() {

	for (size_t i = 0; i < vCBuffers.GetSize(); i++) {
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
	
	for (size_t i = 0; i < pCBuffers.GetSize(); i++) {
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

Shader::Shader(const String& vertexFilename, const String& pixelFilename) {
	inputLayout = nullptr;
	vByteCode = nullptr;
	pByteCode = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;

	String vSource = VFS::Get()->ReadTextFile(vertexFilename);
	String pSource = VFS::Get()->ReadTextFile(pixelFilename);

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

	RemoveComments(vSource);
	RemoveComments(pSource);

	ParseStructs(vSource, FD_SHADER_TYPE_VERTEXSHADER);
	ParseStructs(pSource, FD_SHADER_TYPE_PIXELSHADER);


	CreateBuffers();
}

Shader::~Shader() {
	DX_FREE(inputLayout);

	DX_FREE(vertexShader);
	DX_FREE(pixelShader);
	DX_FREE(vByteCode);
	DX_FREE(pByteCode);

	for (size_t i = 0; i < vCBuffers.GetSize(); i++)
		delete vCBuffers[i];

	for (size_t i = 0; i < pCBuffers.GetSize(); i++)
		delete pCBuffers[i];

}

void Shader::Bind() {
	D3DContext::GetDeviceContext()->IASetInputLayout(inputLayout);

	D3DContext::GetDeviceContext()->VSSetShader(vertexShader, 0, 0);
	D3DContext::GetDeviceContext()->PSSetShader(pixelShader, 0, 0);


	for (size_t i = 0; i < vCBuffers.GetSize(); i++) {
		ShaderStructInfo& cb = *vCBuffers[i];
		D3DContext::GetDeviceContext()->VSSetConstantBuffers(cb.semRegister, 1, &cb.buffer);
	}

	for (size_t i = 0; i < pCBuffers.GetSize(); i++) {
		ShaderStructInfo& cb = *pCBuffers[i];
		D3DContext::GetDeviceContext()->PSSetConstantBuffers(cb.semRegister, 1, &cb.buffer);
	}
}

void Shader::SetVSConstantBufferInternalSlot(unsigned int slot, void* data) {
	ShaderStructInfo& cb = *vCBuffers[slot];
	D3D11_MAPPED_SUBRESOURCE sub;
	ZeroMemory(&sub, sizeof(D3D11_MAPPED_SUBRESOURCE));

	D3DContext::GetDeviceContext()->Map((ID3D11Resource*)cb.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
	memcpy(sub.pData, data, cb.structSize);
	D3DContext::GetDeviceContext()->Unmap((ID3D11Resource*)cb.buffer, 0);
}

void Shader::SetPSConstantBufferInternalSlot(unsigned int slot, void* data) {
	ShaderStructInfo& cb = *pCBuffers[slot];
	D3D11_MAPPED_SUBRESOURCE sub;
	ZeroMemory(&sub, sizeof(D3D11_MAPPED_SUBRESOURCE));

	D3DContext::GetDeviceContext()->Map((ID3D11Resource*)cb.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
	memcpy(sub.pData, data, cb.structSize);
	D3DContext::GetDeviceContext()->Unmap((ID3D11Resource*)cb.buffer, 0);
}

void Shader::SetVSConstantBuffer(const String& bufferName, void* data) {
	for (size_t i = 0; i < vCBuffers.GetSize(); i++) {
		if (vCBuffers[i]->name == bufferName) SetVSConstantBufferInternalSlot(i, data);
	}
}

void Shader::SetPSConstantBuffer(const String& bufferName, void* data) {
	for (size_t i = 0; i < pCBuffers.GetSize(); i++) {
		if (pCBuffers[i]->name == bufferName) SetPSConstantBufferInternalSlot(i, data);
	}
}

void Shader::SetVSConstantBuffer(unsigned int slot, void* data) {
	for (size_t i = 0; i < vCBuffers.GetSize(); i++) {
		if (vCBuffers[i]->semRegister == slot) {
			SetVSConstantBufferInternalSlot(slot, data);
		}
	}
}

void Shader::SetPSConstantBuffer(unsigned int slot, void* data) {
	for (size_t i = 0; i < pCBuffers.GetSize(); i++) {
		if (pCBuffers[i]->semRegister == slot) {
			SetPSConstantBufferInternalSlot(i, data);
		}
	}
}

void Shader::SetTexture(unsigned int slot, const Texture* tex) {
	ID3D11ShaderResourceView* view = tex->GetResourceView();
	D3DContext::GetDeviceContext()->PSSetShaderResources(slot, 1, &view);
}