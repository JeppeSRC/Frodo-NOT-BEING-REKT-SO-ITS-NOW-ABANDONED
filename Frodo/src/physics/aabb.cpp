#include "aabb.h"
#include "collision.h"

#define VEC_OP(vec, op, vec2) (vec.x op vec2.x && vec.y op vec2.y && vec.z op vec2.z)

namespace FD {


AABB::AABB(const vec3& min, const vec3& max) {
	this->min = min;
	this->max = max;
}

void AABB::Translate(const vec3& translation) {
	min += translation;
	max += translation;
}

IntersectionData Collision::AABB_AABB_Test(const AABB* a, const AABB* b) {
	IntersectionData res;
	if (VEC_OP(a->max, > , b->min)) {
		if (VEC_OP(b->max, > , a->min)) {
			res.intersects = true;
			if (VEC_OP(a->max, < , b->max)) {
				res.dist = a->max - b->min;
			} else {
				res.dist = b->max - a->min;
			}
		}
	}

	return res;
}

IntersectionData Collision::AABB_Triangle_Test(const AABB* a, const Triangle* b) {
	return IntersectionData();
}

IntersectionData Collision::AABB_Line_Test(const AABB* a, const Line* b) {
	return IntersectionData();
}

IntersectionData Collision::AABB_Ray_Test(const AABB* a, const Ray* b) {
	return IntersectionData();
}

IntersectionData Collision::AABB_Sphere_Test(const AABB* a, const Sphere* b) {
	return IntersectionData();
}
}