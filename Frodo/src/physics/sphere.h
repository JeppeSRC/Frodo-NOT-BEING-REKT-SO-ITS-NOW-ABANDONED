#pragma once

#include <fd.h>
#include <math/math.h>

namespace FD {


class FDAPI Sphere {
public:
	vec3 position;
	float radius;

	Sphere(const vec3& position, float radius);
};
}
