#pragma once
#include <fd.h>
#include <graphics/d3dcontext.h>
#include <util/string.h>
#include <util/list.h>
#include <common.h>

class FDAPI Shader {
private:
	struct ShaderFieldInfo {
		String name;
		FD_SHADER_FIELD_TYPE type;
	};

	struct ShaderStructInfo {
		String name;
		FD_SHADER_TYPE shaderType;
		unsigned int semRegister;
		unsigned int numFields;
		ShaderFieldInfo* fields;
	};

	void RemoveComments(String& source);
	void ParseStructs(String source, FD_SHADER_TYPE type);
	void ParseFields(String& structSource, size_t offset, ShaderStructInfo* cbuffer);
	

private:
	ID3DBlob* vByteCode;
	ID3DBlob* pByteCode;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	List<ShaderStructInfo*> cbuffers;
	
public:
	Shader(const String& vertexFilename, const String& pixelFilename);
	~Shader();

	void Bind();

	inline const void* GetVSBufferPointer() const { return vByteCode->GetBufferPointer(); }
	inline size_t GetVSBufferSize() const { return vByteCode->GetBufferSize(); }
};
