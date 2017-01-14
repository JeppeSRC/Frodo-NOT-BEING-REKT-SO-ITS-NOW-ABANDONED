#pragma once

#include <fd.h>
#include <math/math.h>
#include <graphics/buffer/vertexbuffer.h>
#include <graphics/buffer/indexbuffer.h>

class FDAPI Renderable {
protected:
	vec3 position;
	vec3 rotation;
	vec3 scale;

	Renderable() {}
	Renderable(const vec3& position, const vec3& rotation, const vec3& scale) : position(position), rotation(rotation), scale(scale) {}
public:

	virtual void Render() = 0;

	inline vec3& GetPosition() { return position; }
	inline vec3& GetRotation() { return rotation; }
	inline vec3& GetScale() { return scale; }

	inline mat4 GetTransform() { return mat4::Translate(position) * mat4::Rotate(rotation) * mat4::Scale(scale); }

	inline void SetPosition(const vec3& position) { this->position = position; }
	inline void SetRotation(const vec3& rotation) { this->rotation = rotation; }
	inline void SetScale(const vec3& scale) { this->scale = scale; }
};
