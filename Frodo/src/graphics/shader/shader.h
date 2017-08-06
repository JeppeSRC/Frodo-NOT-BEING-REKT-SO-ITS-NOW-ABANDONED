#pragma once
#include <fd.h>
#include <graphics/d3dcontext.h>
#include <graphics/texture/texture.h>
#include <graphics/texture/sampler.h>
#include <util/string.h>
#include <util/list.h>
#include <graphics/buffer/bufferlayout.h>

namespace FD {

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

		float32 data;

		FD_SHADER_TYPE shader;
	};

	struct ShaderGenBlock {
		String name;
		String code;

		FD_SHADER_TYPE shader;
	};

	List<ShaderGenVariable*> variables;
	List<ShaderGenBlock*> blocks;

	ShaderGenVariable* ShaderGenGetVariableInternal(const String& name, FD_SHADER_TYPE type) const;
	void ShaderGenParseDefinitions(String& source, FD_SHADER_TYPE type);
	void ShaderGenProcessConditions(String& source, FD_SHADER_TYPE type);
	void ShaderGenProcessGeneration(String& source, FD_SHADER_TYPE type);
	bool ShaderGenProcessFunction(String function, FD_SHADER_TYPE type);
	bool ShaderGenIsVariableDefined(const String& name, FD_SHADER_TYPE type);
	bool ShaderGenIsBlockDefined(const String& name, FD_SHADER_TYPE type);
	void ShaderGenGetParametersFromFunction(const String& function, uint_t offset...);
	void ShaderGenAddVariableData(String& source, FD_SHADER_TYPE type);
	void ShaderGenProcessArithmeticOperations(String& source, FD_SHADER_TYPE type);

public:
	struct StructDefinition {
		String name;

		uint32 structSize;

		BufferLayout layout;
	};

	class ConstantBufferSlot {
	public:
		ConstantBufferSlot(uint32 reg = 0, uint32 size = 0, byte* data = nullptr, BufferLayout layout = BufferLayout()) : semRegister(reg), structSize(size), data(data), layout(layout) {}

		uint32 semRegister;
		uint32 structSize;
		byte* data;

		BufferLayout layout;

		void SetElement(const String& name, const void* data) const;
		void SetElement(uint32 index, const void* data) const;
	};

	struct TextureSlot {
		TextureSlot(uint32 reg = 0, uint32 numTextures = 0) : semRegister(reg), numTextures(numTextures) {}
		uint32 semRegister;
		uint32 numTextures;
	};

	typedef TextureSlot SamplerSlot;

private:
	struct ShaderStructInfo {
		String name;
		uint32 semRegister;
		uint32 structSize;

		BufferLayout layout;

		ID3D11Buffer* buffer = nullptr;

		~ShaderStructInfo() { DX_FREE(buffer); }
	};

	struct ShaderTextureInfo {
		String name;
		uint32 semRegister;
		uint32 numTextures;
		FD_SHADER_TEXTURE_TYPE type;
	};

	typedef ShaderTextureInfo ShaderSamplerInfo;

	enum FD_STRUCT_FIELD_TYPE {
		FD_STRUCT_FIELD_TYPE_UNKNOWN,
		FD_STRUCT_FIELD_TYPE_PRIMITVE,
		FD_STRUCT_FIELD_TYPE_STRUCT
	};

	struct ShaderStructFieldType {
		FD_STRUCT_FIELD_TYPE type;

		uint32 size;

		BufferLayout layout;
	};

	ShaderStructFieldType GetStructFieldType(const String& typeName, FD_SHADER_TYPE type);
	void RemoveComments(String& source);
	void ParseStructs(String source, FD_SHADER_TYPE type);
	void CalcStructSize(String fields, uint32* size, BufferLayout* layout, FD_SHADER_TYPE type);
	void ParseTextures(String source);
	void CreateBuffers();


private:
	String vSourceOriginal;
	String pSourceOriginal;
	String gSourceOriginal;

	String vSource;
	String pSource;
	String gSource;

	ID3DBlob* vByteCode;
	ID3DBlob* pByteCode;
	ID3DBlob* gByteCode;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11GeometryShader* geometryShader;

	List<StructDefinition*> vStructs;
	List<StructDefinition*> pStructs;
	List<StructDefinition*> gStructs;

	List<ShaderStructInfo*> vCBuffers;
	List<ShaderStructInfo*> pCBuffers;
	List<ShaderStructInfo*> gCBuffers;

	List<ShaderTextureInfo*> pTextures;
	List<ShaderSamplerInfo*> pSamplers;

	ID3D11InputLayout* inputLayout;

	void SetVSConstantBufferSlotInternal(uint32 slot, const void* data) const;
	void SetPSConstantBufferSlotInternal(uint32 slot, const void* data) const;
	void SetGSConstantBufferSlotInternal(uint32 slot, const void* data) const;

	void Compile(String vSource, String pSource, String gSource, bool geometry);

public:
	Shader(const String& vertexFilename, const String& pixelFilename, const String& geometryFilename, bool src = false);
	~Shader();

	void Bind();

	void SetVSConstantBuffer(const String& bufferName, const void* data) const;
	void SetPSConstantBuffer(const String& bufferName, const void* data) const;
	void SetGSConstantBuffer(const String& bufferName, const void* data) const;
	void SetVSConstantBuffer(uint32 slot, const void* data) const;
	void SetPSConstantBuffer(uint32 slot, const void* data) const;
	void SetGSConstantBuffer(uint32 slot, const void* data) const;
	void SetVSConstantBuffer(const ConstantBufferSlot vCBuffer) const;
	void SetPSConstantBuffer(const ConstantBufferSlot pCBuffer) const;
	void SetGSConstantBuffer(const ConstantBufferSlot pCBuffer) const;
	void SetTexture(uint32 slot, const Texture* tex) const;
	void SetSampler(uint32 slot, const Sampler* sampler) const;

	ConstantBufferSlot GetVSConstantBufferInfo(const String& name) const;
	ConstantBufferSlot GetPSConstantBufferInfo(const String& name) const;
	ConstantBufferSlot GetGSConstantBufferInfo(const String& name) const;

	TextureSlot GetTextureInfo(const String& name) const;
	SamplerSlot GetSamplerInfo(const String& name) const;

	uint32 GetVSConstantBufferSlotByName(const String& bufferName) const;
	uint32 GetPSConstantBufferSlotByName(const String& bufferName) const;
	uint32 GetGSConstantBufferSlotByName(const String& bufferName) const;
	uint32 GetPSTextureSlotByName(const String& textureName) const;

	void ShaderGenSetVariable(const String& name, FD_SHADER_TYPE type, float32 data);
	void ShaderGenUndefVariable(const String& name, FD_SHADER_TYPE type);

	float32  ShaderGenGetVariable(const String& name, FD_SHADER_TYPE type) const;
	String ShaderGenGetBlock(const String& name, FD_SHADER_TYPE typ) const;

	void ShaderGenComplete();

	inline const void* GetVSBufferPointer() const { return vByteCode->GetBufferPointer(); }
	inline uint_t GetVSBufferSize() const { return vByteCode->GetBufferSize(); }
	inline ID3D11InputLayout* GetInputLayout() const { return inputLayout; }

	inline void SetInputLayout(ID3D11InputLayout* layout) { DX_FREE(inputLayout); inputLayout = layout; }

	static String GetFunctionTypeString(FD_SHADER_GEN_FUNCTION_TYPE type);
};

}