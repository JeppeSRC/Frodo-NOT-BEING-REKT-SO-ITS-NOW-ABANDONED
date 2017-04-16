#pragma once

#include <fd.h>
#include <math/vec3.h>

namespace FD {


class FDAPI Ray {
public:
	vec3 position;
	vec3 direction;

public:
	Ray(const vec3& position, const vec3& direction);

};
}