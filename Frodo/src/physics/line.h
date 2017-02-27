#pragma once

#include <fd.h>
#include <math/math.h>

class FDAPI Line {
public:
	vec3 v0;
	vec3 v1;

public:
	Line(const vec3& v0, const vec3& v1);

	inline vec3 GetDirection() const { return v1 - v0; }
	
};
