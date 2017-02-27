#include "ray.h"

Ray::Ray(const vec3& position, const vec3& direction) {
	this->position = position;
	this->direction = direction;
}