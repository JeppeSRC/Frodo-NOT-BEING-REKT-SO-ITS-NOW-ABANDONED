#pragma once

#include <fd.h>
#include <math/math.h>

class FDAPI Camera {
protected:
	vec3 position;
	vec3 rotation;

	mat4 viewMatrix;


	virtual void UpdateViewMatrix();

public:
	Camera(const vec3& position, const vec3& rotation = vec3(0, 0, 0)) : position(position), rotation(rotation) { }

	inline void SetPosition(const vec3& position) { this->position = position; UpdateViewMatrix(); }
	inline void SetRotation(const vec3& rotation) { this->rotation = rotation; UpdateViewMatrix(); }

	inline const mat4& GetViewMatrix() const { return viewMatrix; }
	inline vec3 GetPosition() const { return position; }
	inline vec3 GetRotation() const { return position; }
};

