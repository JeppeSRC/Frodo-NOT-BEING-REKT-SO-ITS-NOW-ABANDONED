#pragma once
#include <fd.h>
#include <graphics/d3dcontext.h>
#include <graphics/texture/texture.h>
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
	
	ID3D11InputLayout* inputLayout;

	void SetVSConstantBufferInternalSlot(unsigned int slot, void* data);
	void SetPSConstantBufferInternalSlot(unsigned int slot, void* data);

public:
	Shader(const String& vertexFilename, const String& pixelFilename);
	~Shader();

	void Bind();

	void SetVSConstantBuffer(const String& bufferName, void* data);
	void SetPSConstantBuffer(const String& bufferName, void* data);
	void SetVSConstantBuffer(unsigned int slot, void* data);
	void SetPSConstantBuffer(unsigned int slot, void* data);
	void SetTexture(unsigned int slot, const Texture* tex);

	inline const void* GetVSBufferPointer() const { return vByteCode->GetBufferPointer(); }
	inline size_t GetVSBufferSize() const { return vByteCode->GetBufferSize(); }
	inline ID3D11InputLayout* GetInputLayout() { return inputLayout; }
	
	inline void SetInputLayout(ID3D11InputLayout* layout) { DX_FREE(inputLayout); inputLayout = layout; }
};
