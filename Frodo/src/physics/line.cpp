#include "line.h"
#include "collision.h"

namespace FD {


Line::Line(const vec3& v0, const vec3& v1) {
	this->v0 = v0;
	this->v1 = v1;
}


IntersectionData Collision::Line_Line_Test(const Line* a, const Line* b) {
	return IntersectionData();
}

IntersectionData Collision::Line_Ray_Test(const Line* a, const Ray* b) {
	return IntersectionData();
}

IntersectionData Collision::Line_Sphere_Test(const Line* a, const Sphere* b) {
	return IntersectionData();
}
}