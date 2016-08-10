#pragma once

#include "model.h"

class FDAPI MeshFactory {
public:

	static inline Model* CreatePlane(const vec2& size) { return CreatePlane(size.x, size.y); }
	static inline Model* CreateCube(const vec3& size) { return CreateCube(size.x, size.y, size.z); }

	static Model* CreatePlane(float width, float height);
	static Model* CreateCube(float width, float height, float depth);
};
