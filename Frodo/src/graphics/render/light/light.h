#pragma once

#include <fd.h>
#include <math/vec3.h>


class FDAPI Light {
protected:

	vec3 position;
	vec3 direction;
	vec3 color;

	Light(const vec3& position, const vec3& direction, const vec3& color) { this->position = position; this->direction = direction; this->color = color; }

public:


	inline vec3& GetPosition() { return position; }
	inline vec3& GetDirection() { return direction; }
	inline vec3& GetColor() { return color; }
};