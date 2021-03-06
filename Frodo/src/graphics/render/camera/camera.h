#pragma once

#include <fd.h>
#include <math/math.h>

namespace FD {

class FDAPI Camera {
protected:
	vec3 position;
	vec3 rotation;
	vec3 velocity;

	mat4 viewMatrix;
	mat4 projectionMatrix;


	virtual void UpdateViewMatrix();

public:
	Camera(const vec3& position, const vec3& rotation = vec3(0, 0, 0)) : position(position), rotation(rotation) { UpdateViewMatrix(); }

	inline void SetPosition(const vec3& position) { this->position = position; UpdateViewMatrix(); }
	inline void SetRotation(const vec3& rotation) { this->rotation = rotation; UpdateViewMatrix(); }
	inline void SetProjection(const mat4& matrix) { this->projectionMatrix = matrix; }

	inline mat4 GetProjectionMatrix() const { return projectionMatrix; }
	inline mat4 GetViewMatrix() { return viewMatrix; }
	inline vec3 GetPosition() const { return position; }
	inline vec3 GetRotation() const { return rotation; }
	inline vec3 GetVelocity() const { return velocity; }

	inline vec3 GetRight() const {
		const float32* d = viewMatrix.GetData();
		return vec3(d[0 + 0 * 4], d[0 + 1 * 4], d[0 + 2 * 4]);
	}

	inline vec3 GetUp() const {
		const float32* d = viewMatrix.GetData();
		return vec3(d[1 + 0 * 4], d[1 + 1 * 4], d[1 + 2 * 4]);
	}

	inline vec3 GetForward() const {
		const float32* d = viewMatrix.GetData();
		return vec3(d[2 + 0 * 4], d[2 + 1 * 4], d[2 + 2 * 4]);
	}
};

}