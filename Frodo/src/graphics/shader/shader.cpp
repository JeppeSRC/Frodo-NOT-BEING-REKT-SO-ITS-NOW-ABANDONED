#include "shader.h"
#include <d3dcompiler.h>
#include <core/log.h>

FDShader::FDShader(const String& vertexFilename, const String& pixelFilename) {
	vByteCode = nullptr;
	pByteCode = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;

	ID3DBlob* error = nullptr;

	FD_DEBUG("Compiling vertexshader \"%s\"", *vertexFilename);

	D3DCompileFromFile(vertexFilename.GetWCHAR(), 0, 0, "vsMain", "vs_5_0", 0, 0, &vByteCode, &error);

	if (error) {
		FD_FATAL("VertexShader ERROR: %s", error->GetBufferPointer());
		DX_FREE(error);
		FD_ASSERT(vByteCode && "VertexShader failed to compile");
	}

	DX_FREE(error);

	FD_DEBUG("Compiling pixelshader \"%s\"", *pixelFilename);

	D3DCompileFromFile(pixelFilename.GetWCHAR(), 0, 0, "psMain", "ps_5_0", 0, 0, &pByteCode, &error);

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