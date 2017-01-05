#pragma once
#include <fd.h>
#include <graphics/d3dcontext.h>
#include <graphics/texture/texture.h>
#include <util/string.h>
#include <util/list.h>

enum FD_SHADER_TYPE {
	FD_SHADER_TYPE_UNKOWN,
	FD_SHADER_TYPE_VERTEXSHADER,
	FD_SHADER_TYPE_PIXELSHADER,
	FD_SHADER_TYPE_GEOMETRYSHADER,
	FD_SHADER_TYPE_COMPUTESHADER
};

enum FD_SHADER_TEXTURE_TYPE {
	FD_SHADER_TEXTURE_TYPE_TEXTURE1D,
	FD_SHADER_TEXTURE_TYPE_TEXTURE2D,
	FD_SHADER_TEXTURE_TYPE_TEXTURE3D,
	FD_SHADER_TEXTURE_TYPE_TEXTURECUBE,
	FD_SHADER_TEXTURE_TYPE_TEXTURE1D_ARRAY,
	FD_SHADER_TEXTURE_TYPE_TEXTURE2D_ARRAY,
	FD_SHADER_TEXTURE_TYPE_TEXTURECUBE_ARRAY
};

enum FD_SHADER_GEN_FUNCTION_TYPE {
	FD_TRUE,
	FD_FALSE,
	FD_DEFINED,

	FD_EQ,
	FD_NEQ,
	FD_GR,
	FD_LS,
	FD_GE,
	FD_LE
};

class FDAPI Shader {
private:
	struct ShaderGenVariable {
		String name;

		float data;

		FD_SHADER_TYPE shader;
	};

	struct ShaderGenBlock {
		String name;
		String code;

		FD_SHADER_TYPE shader;
	};

	List<ShaderGenVariable*> variables;
	List<ShaderGenBlock*> blocks;

	ShaderGenVariable* ShaderGenGetVariableInternal(const String& name, FD_SHADER_TYPE type);
	void ShaderGenParseDefinitions(String& source, FD_SHADER_TYPE type);
	void ShaderGenProcessConditions(String& source, FD_SHADER_TYPE type);
	void ShaderGenProcessGeneration(String& source, FD_SHADER_TYPE type);
	bool ShaderGenProcessFunction(String function, FD_SHADER_TYPE type);
	bool ShaderGenIsVariableDefined(const String& name, FD_SHADER_TYPE type);
	bool ShaderGenIsBlockDefined(const String& name, FD_SHADER_TYPE type);
	void ShaderGenGetParametersFromFunction(const String& function, size_t offset...);
	void ShaderGenAddVariableData(String& source, FD_SHADER_TYPE type);
	void ShaderGenProcessArithmeticOperations(String& source, FD_SHADER_TYPE type);


private:
	struct ShaderStructInfo {
		String name;
		unsigned int semRegister;
		unsigned int structSize;
		ID3D11Buffer* buffer = nullptr;

		~ShaderStructInfo() { DX_FREE(buffer); }
	};

	struct ShaderTextureInfo {
		String name;
		unsigned int semRegister;
		unsigned int numTextures;
		FD_SHADER_TEXTURE_TYPE type;
	};

	void RemoveComments(String& source);

	void ParseStructs(String source, FD_SHADER_TYPE type);
	void CalcStructSize(String& structSource, size_t offset, ShaderStructInfo* cbuffer);

	void ParseTextures(String source);

	void CreateBuffers();
	

private:
	String vSourceOriginal;
	String pSourceOriginal;

	String vSource;
	String pSource;


	ID3DBlob* vByteCode;
	ID3DBlob* pByteCode;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	List<ShaderStructInfo*> vCBuffers;
	List<ShaderStructInfo*> pCBuffers;

	List<ShaderTextureInfo*> pTextures;
	
	ID3D11InputLayout* inputLayout;

	void SetVSConstantBufferSlotInternal(unsigned int slot, void* data);
	void SetPSConstantBufferSlotInternal(unsigned int slot, void* data);

	void Compile(String vSource, String pSource);

public:
	Shader(const String& vertexFilename, const String& pixelFilename, bool src = false);
	~Shader();

	void Bind();

	void SetVSConstantBuffer(const String& bufferName, void* data);
	void SetPSConstantBuffer(const String& bufferName, void* data);
	void SetVSConstantBuffer(unsigned int slot, void* data);
	void SetPSConstantBuffer(unsigned int slot, void* data);
	void SetTexture(unsigned int slot, const Texture* tex);

	unsigned int GetVSConstantBufferSlotByName(const String& bufferName);
	unsigned int GetPSConstantBufferSlotByName(const String& bufferName);
	unsigned int GetPSTextureSlotByName(const String& textureName);

	void ShaderGenSetVariable(const String& name, FD_SHADER_TYPE type, float data);
	void ShaderGenUndefVariable(const String& name, FD_SHADER_TYPE type);

	float  ShaderGenGetVariable(const String& name, FD_SHADER_TYPE type);
	String ShaderGenGetBlock(const String& name, FD_SHADER_TYPE typ);

	void ShaderGenComplete();

	inline const void* GetVSBufferPointer() const { return vByteCode->GetBufferPointer(); }
	inline size_t GetVSBufferSize() const { return vByteCode->GetBufferSize(); }
	inline ID3D11InputLayout* GetInputLayout() { return inputLayout; }
	
	inline void SetInputLayout(ID3D11InputLayout* layout) { DX_FREE(inputLayout); inputLayout = layout; }

	static String GetFunctionTypeString(FD_SHADER_GEN_FUNCTION_TYPE type);
};
