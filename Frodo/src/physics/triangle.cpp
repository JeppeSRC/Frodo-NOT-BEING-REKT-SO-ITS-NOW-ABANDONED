#include "triangle.h"
#include "collision.h"

namespace FD {


Triangle::Triangle(const vec3& v0, const vec3& v1, const vec3& v2) {
	this->v0 = v0;
	this->v1 = v1;
	this->v2 = v2;
}

IntersectionData Collision::Triangle_Triangle_Test(const Triangle* a, const Triangle* b) {
	return IntersectionData();
}

IntersectionData Collision::Triangle_Line_Test(const Triangle* a, const Line* b) {
	return IntersectionData();
}

IntersectionData Collision::Triangle_Ray_Test(const Triangle* a, const Ray* b) {
	return IntersectionData();
}

IntersectionData Collision::Triangle_Sphere_Test(const Triangle* a, const Sphere* b) {
	return IntersectionData();
}

}