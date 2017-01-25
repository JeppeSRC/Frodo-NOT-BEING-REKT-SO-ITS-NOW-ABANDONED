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

	virtual void SetTexture(const String& name, Texture* texture);
	virtual void SetVCBuffer(const String& name, void* data);
	virtual void SetPCBuffer(const String& name, void* data);

	void SetVCBufferElement(const String& name, void* data);
	void SetPCBufferElement(const String& name, void* data);

	inline void SetVCBufferElement(const String& name, float data) { SetVCBufferElement(name, &data); }
	inline void SetPCBufferElement(const String& name, float data) { SetPCBufferElement(name, &data); }
	inline void SetVCBufferElement(const String& name, vec2 data) { SetVCBufferElement(name, &data); }
	inline void SetPCBufferElement(const String& name, vec2 data) { SetPCBufferElement(name, &data); }
	inline void SetVCBufferElement(const String& name, vec3 data) { SetVCBufferElement(name, &data); }
	inline void SetPCBufferElement(const String& name, vec3 data) { SetPCBufferElement(name, &data); }
	inline void SetVCBufferElement(const String& name, vec4 data) { SetVCBufferElement(name, &data); }
	inline void SetPCBufferElement(const String& name, vec4 data) { SetPCBufferElement(name, &data); }
	inline void SetVCBufferElement(const String& name, mat3 data) { SetVCBufferElement(name, &data); }
	inline void SetPCBufferElement(const String& name, mat4 data) { SetPCBufferElement(name, &data); }

	void SetVCBufferElement(uint32 index, void* data);
	void SetPCBufferElement(uint32 index, void* data);

	inline void SetVCBufferElement(const uint32 index, float data) { SetVCBufferElement(index, &data); }
	inline void SetPCBufferElement(const uint32 index, float data) { SetPCBufferElement(index, &data); }
	inline void SetVCBufferElement(const uint32 index, vec2 data) { SetVCBufferElement(index, &data); }
	inline void SetPCBufferElement(const uint32 index, vec2 data) { SetPCBufferElement(index, &data); }
	inline void SetVCBufferElement(const uint32 index, vec3 data) { SetVCBufferElement(index, &data); }
	inline void SetPCBufferElement(const uint32 index, vec3 data) { SetPCBufferElement(index, &data); }
	inline void SetVCBufferElement(const uint32 index, vec4 data) { SetVCBufferElement(index, &data); }
	inline void SetPCBufferElement(const uint32 index, vec4 data) { SetPCBufferElement(index, &data); }
	inline void SetVCBufferElement(const uint32 index, mat3 data) { SetVCBufferElement(index, &data); }
	inline void SetPCBufferElement(const uint32 index, mat4 data) { SetPCBufferElement(index, &data); }

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

	virtual void SetTexture(const String& name, Texture* texture);
	virtual void SetVCBuffer(const String& name, void* data);
	virtual void SetPCBuffer(const String& name, void* data);

	void SetVCBufferElement(const String& name, void* data);
	void SetPCBufferElement(const String& name, void* data);

	inline void SetVCBufferElement(const String& name, float data) { SetVCBufferElement(name, &data); }
	inline void SetPCBufferElement(const String& name, float data) { SetPCBufferElement(name, &data); }
	inline void SetVCBufferElement(const String& name, vec2 data) { SetVCBufferElement(name, &data); }
	inline void SetPCBufferElement(const String& name, vec2 data) { SetPCBufferElement(name, &data); }
	inline void SetVCBufferElement(const String& name, vec3 data) { SetVCBufferElement(name, &data); }
	inline void SetPCBufferElement(const String& name, vec3 data) { SetPCBufferElement(name, &data); }
	inline void SetVCBufferElement(const String& name, vec4 data) { SetVCBufferElement(name, &data); }
	inline void SetPCBufferElement(const String& name, vec4 data) { SetPCBufferElement(name, &data); }
	inline void SetVCBufferElement(const String& name, mat3 data) { SetVCBufferElement(name, &data); }
	inline void SetPCBufferElement(const String& name, mat4 data) { SetPCBufferElement(name, &data); }

	void SetVCBufferElement(uint32 index, void* data);
	void SetPCBufferElement(uint32 index, void* data);

	inline void SetVCBufferElement(const uint32 index, float data) { SetVCBufferElement(index, &data); }
	inline void SetPCBufferElement(const uint32 index, float data) { SetPCBufferElement(index, &data); }
	inline void SetVCBufferElement(const uint32 index, vec2 data) { SetVCBufferElement(index, &data); }
	inline void SetPCBufferElement(const uint32 index, vec2 data) { SetPCBufferElement(index, &data); }
	inline void SetVCBufferElement(const uint32 index, vec3 data) { SetVCBufferElement(index, &data); }
	inline void SetPCBufferElement(const uint32 index, vec3 data) { SetPCBufferElement(index, &data); }
	inline void SetVCBufferElement(const uint32 index, vec4 data) { SetVCBufferElement(index, &data); }
	inline void SetPCBufferElement(const uint32 index, vec4 data) { SetPCBufferElement(index, &data); }
	inline void SetVCBufferElement(const uint32 index, mat3 data) { SetVCBufferElement(index, &data); }
	inline void SetPCBufferElement(const uint32 index, mat4 data) { SetPCBufferElement(index, &data); }

	inline Shader::ConstantBufferSlot GetVCBuffer() const { return vCBuffer; }
	inline Shader::ConstantBufferSlot GetPCBuffer() const { return pCBuffer; }

	inline Shader* GetShader() { return shader; }
	inline Material* GetMaterial() { return parent; }

	inline const Map<uint32, Texture*>& GetTextures() const { return textures; }
};