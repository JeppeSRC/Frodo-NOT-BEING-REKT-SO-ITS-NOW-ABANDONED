#pragma once
#include <fd.h>
#include <graphics/d3dcontext.h>
#include <graphics/texture/texture.h>
#include <util/string.h>
#include <util/list.h>
#include <graphics/buffer/bufferlayout.h>

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

	ShaderGenVariable* ShaderGenGetVariableInternal(const String& name, FD_SHADER_TYPE type);
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
	class ConstantBufferSlot {
	public:
		ConstantBufferSlot(uint32 reg = 0, uint32 size = 0, byte* data = nullptr, BufferLayout layout = BufferLayout()) : semRegister(reg), structSize(size), data(data), layout(layout) {}
	/*	ConstantBufferSlot(const ConstantBufferSlot& slot) {
			semRegister = slot.semRegister;
			structSize = slot.structSize;
			layout = slot.layout;
			data = new byte[structSize];
			memcpy(data, slot.data, structSize);
		}

		ConstantBufferSlot(ConstantBufferSlot&& slot) {
			data = nullptr;
			*this = std::move(slot);
		}

		ConstantBufferSlot& operator=(ConstantBufferSlot& slot) {
			delete[] data;
			semRegister = slot.semRegister;
			structSize = slot.structSize;
			layout = slot.layout;
			data = new byte[structSize];
			memcpy(data, slot.data, structSize);
			return *this;
		}

		ConstantBufferSlot& operator=(ConstantBufferSlot&& slot) {
			if (this != &slot) {
				delete[] data;
				semRegister = slot.semRegister;
				structSize = slot.structSize;
				layout = slot.layout;

				data = slot.data;

				slot.data = nullptr;
				slot.semRegister = 0;
				slot.structSize = 0;
			}

			return *this;
		}*/
		
		uint32 semRegister;
		uint32 structSize;
		byte* data;

		BufferLayout layout;

		void SetElement(const String& name, void* data);
		void SetElement(uint32 index, void* data);
	};

	struct TextureSlot {
		TextureSlot(uint32 reg = 0, uint32 numTextures = 0) : semRegister(reg), numTextures(numTextures) {}
		uint32 semRegister;
		uint32 numTextures;
	};

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

	void RemoveComments(String& source);

	void ParseStructs(String source, FD_SHADER_TYPE type);
	void CalcStructSize(String& structSource, uint_t offset, ShaderStructInfo* cbuffer);

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

	void SetVSConstantBufferSlotInternal(uint32 slot, void* data);
	void SetPSConstantBufferSlotInternal(uint32 slot, void* data);

	void Compile(String vSource, String pSource);

public:
	Shader(const String& vertexFilename, const String& pixelFilename, bool src = false);
	~Shader();

	void Bind();

	void SetVSConstantBuffer(const String& bufferName, void* data);
	void SetPSConstantBuffer(const String& bufferName, void* data);
	void SetVSConstantBuffer(uint32 slot, void* data);
	void SetPSConstantBuffer(uint32 slot, void* data);
	void SetVSConstantBuffer(ConstantBufferSlot vCBuffer);
	void SetPSConstantBuffer(ConstantBufferSlot pCBuffer);
	void SetTexture(uint32 slot, const Texture* tex);

	ConstantBufferSlot GetVSConstantBufferInfo(const String& name);
	ConstantBufferSlot GetPSConstantBufferInfo(const String& name);

	TextureSlot GetTextureInfo(const String& name);

	uint32 GetVSConstantBufferSlotByName(const String& bufferName);
	uint32 GetPSConstantBufferSlotByName(const String& bufferName);
	uint32 GetPSTextureSlotByName(const String& textureName);

	void ShaderGenSetVariable(const String& name, FD_SHADER_TYPE type, float32 data);
	void ShaderGenUndefVariable(const String& name, FD_SHADER_TYPE type);

	float32  ShaderGenGetVariable(const String& name, FD_SHADER_TYPE type);
	String ShaderGenGetBlock(const String& name, FD_SHADER_TYPE typ);

	void ShaderGenComplete();

	inline const void* GetVSBufferPointer() const { return vByteCode->GetBufferPointer(); }
	inline uint_t GetVSBufferSize() const { return vByteCode->GetBufferSize(); }
	inline ID3D11InputLayout* GetInputLayout() { return inputLayout; }
	
	inline void SetInputLayout(ID3D11InputLayout* layout) { DX_FREE(inputLayout); inputLayout = layout; }

	static String GetFunctionTypeString(FD_SHADER_GEN_FUNCTION_TYPE type);
};
