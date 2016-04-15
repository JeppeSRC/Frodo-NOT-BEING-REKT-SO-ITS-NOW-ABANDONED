#include "shader.h"
#include <d3dcompiler.h>
#include <core/log.h>
#include <util/fileutils.h>

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

	List<String*> lines = source.Split('\n');

	for (size_t i = 0; i < lines.GetSize(); i++) {
		String& line = *lines[i];
		

		size_t start = line.Find("cbuffer");

		if (start != -1) {
			start += 7;
			size_t colon = line.Find(":", start);
	
			String name(line.str + start, colon - start);
			name.RemoveBlankspace();

			size_t regIndex = line.Find("register", colon) + 10;

			int reg = atoi(line.str + regIndex);
			
			ShaderStructInfo* cbuffer = new ShaderStructInfo;
			
			cbuffer->name = name;
			cbuffer->semRegister = reg;


			
			

		} else {
			continue;
		}
	}

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