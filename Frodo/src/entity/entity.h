#pragma once

#include <fd.h>
#include <graphics/render/mesh/mesh.h>
#include <graphics/render/material/material.h>

namespace FD {

class FDAPI Entity {
protected:
	vec3 position;

public:
	Entity() { }
	Entity(const vec3& position) : position(position){ }
	virtual ~Entity() {}

	virtual void Update() {}

	inline vec3& GetPosition() { return position; }

};

class FDAPI Entity3D : public Entity {
protected:
	Mesh* mesh;

	vec3 rotation;
	vec3 scale;

public:
	Entity3D(const vec3& position, const vec3& rotation, const vec3& scale = vec3(1, 1, 1)) : Entity(position), rotation(rotation), scale(scale) { mesh = nullptr; }

	inline void SetMesh(Mesh* mesh) { this->mesh = mesh; }

	inline Mesh* GetMesh() const { return mesh; }
	inline mat4 GetTransform() const { return mat4::Translate(position) * mat4::Rotate(rotation) * mat4::Scale(scale); }
	inline vec3& GetRotation() { return rotation; }
	inline vec3& GetScale() { return scale; }

};

class FDAPI Sprite : public Entity {
protected:
	vec2 size;
	vec4 color;
	Texture2D* texture;

public:
	Sprite(const vec3& position, const vec2& size, Texture2D* texture) : Entity(position), size(size), texture(texture), color(vec4(1, 1, 1, 1)) {}
	Sprite(const vec3& position, const vec2& size, const vec4& color) : Entity(position), size(size), texture(nullptr), color(color) {}

	inline vec2& GetSize() { return size; }
	inline vec4& GetColor() { return color; }
	inline Texture2D* GetTexture() const { return texture; }

};

}