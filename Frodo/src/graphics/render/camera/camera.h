#pragma once

#include <fd.h>
#include <math/math.h>

namespace FD {

class FDAPI Camera {
protected:
	vec3 position;
	vec3 rotation;

	mat4 viewMatrix;
	mat4 projectionMatrix;


	virtual void UpdateViewMatrix();

public:
	Camera(const vec3& position, const vec3& rotation = vec3(0, 0, 0)) : position(position), rotation(rotation) { UpdateViewMatrix(); }

	inline void SetPosition(const vec3& position) { this->position = position; UpdateViewMatrix(); }
	inline void SetRotation(const vec3& rotation) { this->rotation = rotation; UpdateViewMatrix(); }
	inline void SetProjection(const mat4& matrix) { this->projectionMatrix = matrix; }

	inline const mat4& GetProjectionMatrix() const { return projectionMatrix; }
	inline mat4 GetViewMatrix() { return viewMatrix; }
	inline vec3 GetPosition() const { return position; }
	inline vec3 GetRotation() const { return rotation; }

	inline vec3 GetForward() {
		mat4 rot = mat4::Rotate(rotation);
		const float32* d = rot.GetData();
		return vec3(d[2 + 0 * 4], d[2 + 1 * 4], d[2 + 2 * 4]);
	}
};

}