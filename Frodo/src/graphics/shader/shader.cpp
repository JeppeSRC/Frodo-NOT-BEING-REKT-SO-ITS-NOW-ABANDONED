#include "shader.h"
#include <d3dcompiler.h>
#include <core/log.h>
#include <util/fileutils.h>


void FDShader::RemoveComments(String& source) {

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

void FDShader::ParseStructs(String vSource, String pSource) {

	List<String*> lines = vSource.Split('\n');

	for (size_t i = 0; i < lines.GetSize(); i++) {
		String& line = *lines[i];

		size_t start = line.Find("cbuffer");

		if (start) {

			size_t colon = line.Find(":");


		} else {
			continue;
		}
	}

}

FDShader::FDShader(const String& vertexFilename, const String& pixelFilename) {
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

	ParseStructs(vSource, pSource);

}

FDShader::~FDShader() {
	DX_FREE(vertexShader);
	DX_FREE(pixelShader);
	DX_FREE(vByteCode);
	DX_FREE(pByteCode);
}

void FDShader::Bind() {
	D3DContext::GetDeviceContext()->VSSetShader(vertexShader, 0, 0);
	D3DContext::GetDeviceContext()->PSSetShader(pixelShader, 0, 0);
}