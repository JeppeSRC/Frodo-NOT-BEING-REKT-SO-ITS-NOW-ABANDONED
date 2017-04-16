#include "sphere.h"
#include "collision.h"

namespace FD {


Sphere::Sphere(const vec3& position, float radius) {
	this->position = position;
	this->radius = radius;
}

IntersectionData Collision::Sphere_Sphere_Test(const Sphere* a, const Sphere* b) {
	return IntersectionData();
}

}