#pragma once

#include <fd.h>
#include <math/math.h>

#include "aabb.h"
#include "triangle.h"
#include "line.h"
#include "ray.h"
#include "sphere.h"

namespace FD {


struct IntersectionData {
	vec3 dist;

	bool intersects;
};

class FDAPI Collision {
public:

	static IntersectionData AABB_AABB_Test(const AABB* a, const AABB* b);
	static IntersectionData AABB_Triangle_Test(const AABB* a, const Triangle* b);
	static IntersectionData AABB_Line_Test(const AABB* a, const Line* b);
	static IntersectionData AABB_Ray_Test(const AABB* a, const Ray* b);
	static IntersectionData AABB_Sphere_Test(const AABB* a, const Sphere* b);

	inline static IntersectionData Triangle_AABB_Test(const Triangle* a, const AABB* b) { return AABB_Triangle_Test(b, a); }
	static IntersectionData Triangle_Triangle_Test(const Triangle* a, const Triangle* b);
	static IntersectionData Triangle_Line_Test(const Triangle* a, const Line* b);
	static IntersectionData Triangle_Ray_Test(const Triangle* a, const Ray* b);
	static IntersectionData Triangle_Sphere_Test(const Triangle* a, const Sphere* b);

	inline static IntersectionData Line_AABB_Test(const Line* a, const AABB* b) { return AABB_Line_Test(b, a); }
	inline static IntersectionData Line_Triangle_Test(const Line* a, const Triangle* b) { return Triangle_Line_Test(b, a); }
	static IntersectionData Line_Line_Test(const Line* a, const Line* b);
	static IntersectionData Line_Ray_Test(const Line* a, const Ray* b);
	static IntersectionData Line_Sphere_Test(const Line* a, const Sphere* b);

	inline static IntersectionData Ray_AABB_Test(const Ray* a, const AABB* b) { return AABB_Ray_Test(b, a); }
	inline static IntersectionData Ray_Triangle_Test(const Ray* a, const Triangle* b) { return Triangle_Ray_Test(b, a); }
	inline static IntersectionData Ray_Line_Test(const Ray* a, const Line* b) { return Line_Ray_Test(b, a); }
	static IntersectionData Ray_Ray_Test(const Ray* a, const Ray* b);
	static IntersectionData Ray_Sphere_Test(const Ray* a, const Sphere* b);

	inline static IntersectionData Sphere_AABB_Test(const Sphere* a, const AABB* b) { return AABB_Sphere_Test(b, a); }
	inline static IntersectionData Sphere_Triangle_Test(const Sphere* a, const Triangle* b) { return Triangle_Sphere_Test(b, a); }
	inline static IntersectionData Sphere_Line_Test(const Sphere* a, const Line* b) { return Line_Sphere_Test(b, a); }
	inline static IntersectionData Sphere_Ray_Test(const Sphere* a, const Ray* b) { return Ray_Sphere_Test(b, a); }
	static IntersectionData Sphere_Sphere_Test(const Sphere* a, const Sphere* b);
};
}