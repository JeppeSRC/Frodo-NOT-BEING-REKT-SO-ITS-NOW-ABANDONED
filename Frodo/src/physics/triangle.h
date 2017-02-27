#pragma once

#include <fd.h>
#include <math/vec3.h>

class FDAPI Triangle {
public:
	vec3 v0;
	vec3 v1;
	vec3 v2;

public:
	Triangle(const vec3& v0, const vec3& v1, const vec3& v2);


};
