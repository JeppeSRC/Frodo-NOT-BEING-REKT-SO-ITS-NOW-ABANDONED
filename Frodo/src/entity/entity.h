#pragma once

#include <fd.h>
#include <graphics/texture/texture2d.h>
#include <graphics/render/mesh/mesh.h>
#include <graphics/render/material/material.h>

namespace FD {

enum FD_ENTITY_FLAGS {
	
};

class FDAPI Entity {
protected:
	vec3 position;

	uint32 flags;
public:
	Entity() { }
	Entity(const vec3& position) : position(position){ }
	virtual ~Entity() {}

	virtual void Update() {}

	inline void SetPosition(const vec3& position) { this->position = position; }
	inline void SetFlag(FD_ENTITY_FLAGS flag) { flags |= flag; }

	inline const vec3& GetPosition() const { return position; }
	inline uint32 GetFlags() const { return flags; }
};

class FDAPI Entity3D : public Entity {
protected:
	Mesh* mesh;

	vec3 rotation;
	vec3 scale;

public:
	Entity3D(const vec3& position, const vec3& rotation, const vec3& scale = vec3(1, 1, 1)) : Entity(position), rotation(rotation), scale(scale) { mesh = nullptr; }

	inline void SetMesh(Mesh* mesh) { this->mesh = mesh; }

	inline void SetRotation(const vec3& rotation) { this->rotation = rotation; }
	inline void SetScale(const vec3& scale) { this->scale = scale; }

	inline Mesh* GetMesh() const { return mesh; }
	inline mat4 GetTransform() const { return mat4::Translate(position) * mat4::Rotate(rotation) * mat4::Scale(scale); }
	inline const vec3& GetRotation() const { return rotation; }
	inline const vec3& GetScale() const { return scale; }

};

class FDAPI Sprite : public Entity {
protected:
	vec2 size;
	vec4 color;
	Texture2D* texture;

public:
	Sprite(const vec3& position, const vec2& size, Texture2D* texture) : Entity(position), size(size), texture(texture), color(vec4(1, 1, 1, 1)) {}
	Sprite(const vec3& position, const vec2& size, const vec4& color) : Entity(position), size(size), texture(nullptr), color(color) {}

	inline void SetSize(const vec2& size) { this->size = size; }
	inline void SetColor(const vec4& color) { this->color = color; }

	inline const vec2& GetSize() const { return size; }
	inline const vec4& GetColor() const { return color; }
	inline Texture2D* GetTexture() const { return texture; }

};

}