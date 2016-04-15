#include "shader.h"
#include <d3dcompiler.h>
#include <core/log.h>
#include <util/fileutils.h>

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
		FD_DEBUG("%u", cbufferStart);
		if (cbufferStart < 7) break;

		size_t colon = source.Find(":", cbufferStart);

		String name(source.str + cbufferStart, colon - cbufferStart);
		name.RemoveBlankspace();

		size_t regIndex = source.Find("register", colon) + 10;

		ShaderStructInfo* cbuffer = new ShaderStructInfo;

		cbuffer->name = name;
		cbuffer->semRegister = atoi(*source + regIndex);
		cbuffer->shaderType = type;

		ParseFields(source, cbufferStart-7, cbuffer);

		cbuffers.Push_back(cbuffer);
	}

	
}

void Shader::ParseFields(String& structSource, size_t offset, ShaderStructInfo* cbuffer) {

	FD_SHADER_FIELD_TYPE types[6]{
		FD_SHADER_FIELD_TYPE_MAT4,
		FD_SHADER_FIELD_TYPE_MAT3,
		FD_SHADER_FIELD_TYPE_VEC4,
		FD_SHADER_FIELD_TYPE_VEC3,
		FD_SHADER_FIELD_TYPE_VEC2,
		FD_SHADER_FIELD_TYPE_FLOAT};

	size_t end = structSource.Find("};", offset);

	String fields(*structSource + offset, end - offset);

	cbuffer->numFields = fields.Count(";");
	cbuffer->fields = new ShaderFieldInfo[cbuffer->numFields];

	size_t currSemicolon = fields.Find(";");
	size_t fieldOffset = 0;

	for (size_t num = 0; num < cbuffer->numFields; num++) {
		for (size_t i = 0; i < 6; i++) {
			size_t index = fields.Find(get_field_type_as_string(types[i]), fieldOffset);

			if (index > currSemicolon || index == -1) continue;

			size_t nameStart = fields.Find(" ", index);

			String name(*fields + nameStart, currSemicolon - nameStart);
			name.RemoveBlankspace();

			cbuffer->fields[num].name = name;
			cbuffer->fields[num].type = types[i];

			fieldOffset = currSemicolon;
			currSemicolon = fields.Find(";", currSemicolon + 1);

			i = 6;
		}
	}

	structSource.Remove(offset, end+2);
}

Shader::Shader(const String& vertexFilename, const String& pixelFilename) {
	vByteCode = nullptr;
	pByteCode = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;

	String vSource = FDReadTextFile(vertexFilename);
	String pSource = FDReadTextFile(pixelFilename);

	ID3DBlob* error = nullptr;

	FD_DEBUG("Compiling vertexshader \"%s\"", *vertexFilename);

	D3DCompile(*vSource, vSource.length, 0, 0, 0, "vsMain", "vs_5_0", 0, 0, &vByteCode, &error);

	if (error) {
		FD_FATAL("VertexShader ERROR: %s", error->GetBufferPointer());
		DX_FREE(error);
		FD_ASSERT(vByteCode && "VertexShader failed to compile");
	}

	DX_FREE(error);

	FD_DEBUG("Compiling pixelshader \"%s\"", *pixelFilename);

	D3DCompile(*pSource, pSource.length, 0, 0, 0, "psMain", "ps_5_0", 0, 0, &pByteCode, &error);

	if (error) {
		FD_FATAL("PixelShader ERROR: %s", error->GetBufferPointer());
		DX_FREE(error);
		FD_ASSERT(pByteCode && "PixelShader failed to compile");
	}

	FD_DEBUG("Shaders compiled");

	DX_FREE(error);
	

	D3DContext::GetDevice()->CreateVertexShader(vByteCode->GetBufferPointer(), vByteCode->GetBufferSize(), 0, &vertexShader);

	FD_ASSERT(vertexShader && "Failed to create vertexshader");

	D3DContext::GetDevice()->CreatePixelShader(pByteCode->GetBufferPointer(), pByteCode->GetBufferSize(), 0, &pixelShader);

	FD_ASSERT(pixelShader && "Failed to create pixelshader");

	RemoveComments(vSource);
	RemoveComments(pSource);

	ParseStructs(vSource, FD_SHADER_TYPE_VERTEXSHADER);
	ParseStructs(pSource, FD_SHADER_TYPE_PIXELSHADER);

}

Shader::~Shader() {
	DX_FREE(vertexShader);
	DX_FREE(pixelShader);
	DX_FREE(vByteCode);
	DX_FREE(pByteCode);
}

void Shader::Bind() {
	D3DContext::GetDeviceContext()->VSSetShader(vertexShader, 0, 0);
	D3DContext::GetDeviceContext()->PSSetShader(pixelShader, 0, 0);
}