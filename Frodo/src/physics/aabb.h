#pragma once

#include <fd.h>
#include <math/vec3.h>

class FDAPI AABB {
public:
	vec3 min;
	vec3 max;

public:
	AABB(const vec3& min, const vec3& max);

	void Translate(const vec3& translation);

	inline vec3 GetCenter() const { return max - min; }
};

