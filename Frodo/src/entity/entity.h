#pragma once

#include <fd.h>
#include <graphics/render/mesh/mesh.h>
#include <graphics/render/material/material.h>

class FDAPI Entity {
protected:
	Mesh* mesh;

	vec3 position;
	vec3 rotation;
	vec3 scale;

public:
	Entity() { mesh = nullptr; }
	Entity(const vec3& position, const vec3& rotation, const vec3& scale = vec3(1, 1, 1)) : position(position), rotation(rotation), scale(scale) { mesh = nullptr; }
	virtual ~Entity() { }
	
	virtual void Update() {}

	inline Mesh* GetMesh() const { return mesh; }
	inline mat4 GetTransform() const { return mat4::Translate(position) * mat4::Rotate(rotation) * mat4::Scale(scale); }

	inline vec3& GetPosition() { return position; }
	inline vec3& GetRotation() { return rotation; }
	inline vec3& GetScale() { return scale; }
};