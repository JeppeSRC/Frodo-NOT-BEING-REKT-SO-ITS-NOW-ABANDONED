#pragma once
#include <fd.h>
#include <graphics/d3dcontext.h>
#include <util/string.h>
#include <util/list.h>
#include <common.h>

class FDAPI Shader {
private:
	struct ShaderStructInfo {
		String name;
		unsigned int semRegister;
		unsigned int structSize;
		ID3D11Buffer* buffer = nullptr;

		~ShaderStructInfo() { DX_FREE(buffer); }
	};

	void RemoveComments(String& source);
	void ParseStructs(String source, FD_SHADER_TYPE type);
	void CalcStructSize(String& structSource, size_t offset, ShaderStructInfo* cbuffer);

	void CreateBuffers();
	

private:
	ID3DBlob* vByteCode;
	ID3DBlob* pByteCode;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	List<ShaderStructInfo*> vCBuffers;
	List<ShaderStructInfo*> pCBuffers;
	
public:
	Shader(const String& vertexFilename, const String& pixelFilename);
	~Shader();

	void Bind();

	void SetVSConstantBuffer(unsigned int slot, void* data);
	void SetPSConstantBuffer(unsigned int slot, void* data);

	inline const void* GetVSBufferPointer() const { return vByteCode->GetBufferPointer(); }
	inline size_t GetVSBufferSize() const { return vByteCode->GetBufferSize(); }
};
