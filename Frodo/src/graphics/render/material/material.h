#pragma once

#include <fd.h>
#include <graphics/texture/texture.h>
#include <math/math.h>
#include <util/map.h>
#include <graphics/shader/shader.h>
#include <graphics/buffer/bufferlayout.h>

class FDAPI Material {
protected:
	Shader* shader;

	Shader::ConstantBufferSlot vCBuffer;
	Shader::ConstantBufferSlot pCBuffer;

	Map<uint32, Texture*> textures;
public:
	Material(Shader* shader);
	virtual ~Material();

	virtual void Bind();

	void SetTexture(const String& name, Texture* texture);
	void SetVCBuffer(const String& name, void* data);
	void SetPCBuffer(const String& name, void* data);

	void SetVCBufferElement(const String& name, void* data);
	void SetPCBufferElement(const String& name, void* data);
	void SetVCBufferElement(uint32 index, void* data);
	void SetPCBufferElement(uint32 index, void* data);

	inline Shader::ConstantBufferSlot GetVCBuffer() const { return vCBuffer; }
	inline Shader::ConstantBufferSlot GetPCBuffer() const { return pCBuffer; }

	inline Shader* GetShader() { return shader; }

	inline const Map<uint32, Texture*>& GetTextures() const { return textures; }
};

class FDAPI MaterialInstance {
protected:
	Material* parent = nullptr;

	Shader* shader;

	Shader::ConstantBufferSlot vCBuffer;
	Shader::ConstantBufferSlot pCBuffer;

	Map<uint32, Texture*> textures;
public:
	MaterialInstance(Material* material);
	virtual ~MaterialInstance();

	virtual void Bind();

	void SetTexture(const String& name, Texture* texture);
	void SetVCBuffer(const String& name, void* data);
	void SetPCBuffer(const String& name, void* data);

	void SetVCBufferElement(const String& name, void* data);
	void SetPCBufferElement(const String& name, void* data);
	void SetVCBufferElement(uint32 index, void* data);
	void SetPCBufferElement(uint32 index, void* data);

	inline Shader::ConstantBufferSlot GetVCBuffer() const { return vCBuffer; }
	inline Shader::ConstantBufferSlot GetPCBuffer() const { return pCBuffer; }

	inline Shader* GetShader() { return shader; }
	inline Material* GetMaterial() { return parent; }

	inline const Map<uint32, Texture*>& GetTextures() const { return textures; }
};