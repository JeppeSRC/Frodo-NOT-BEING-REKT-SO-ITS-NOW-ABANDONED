#pragma once
#include <fd.h>
#include <graphics/d3dcontext.h>
#include <util/string.h>

class FDAPI FDShader {
private:
	ID3DBlob* vByteCode;
	ID3DBlob* pByteCode;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	
public:
	FDShader(const String& vertexFilename, const String& pixelFilename);
	~FDShader();

	void Bind();

	inline const void* GetVSBufferPointer() const { return vByteCode->GetBufferPointer(); }
	inline size_t GetVSBufferSize() const { return vByteCode->GetBufferSize(); }
};
