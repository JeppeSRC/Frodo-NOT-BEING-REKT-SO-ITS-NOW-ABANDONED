#include "ray.h"
#include "collision.h"

namespace FD {


Ray::Ray(const vec3& position, const vec3& direction) {
	this->position = position;
	this->direction = direction;
}

IntersectionData Collision::Ray_Ray_Test(const Ray* a, const Ray* b) {
	return IntersectionData();
}

IntersectionData Collision::Ray_Sphere_Test(const Ray* a, const Sphere* b) {
	return IntersectionData();
}

}