#pragma once

#include "model.h"
#include <util/list.h>

class FDAPI MeshFactory {
private:

	template<uint32 N>
	struct Face {
		struct Vertex {
			uint32 vertex;
			uint32 texCoord;
			uint32 normal;
		} v[N];

		inline Vertex& operator[](uint_t index) { return v[index]; }
	};

	static void ParseOBJ(const String obj, List<vec3>& vertices, List<vec2>& texCoords, List<vec3>& normals, List<uint32>& indices);
	static void MakeFacesOBJ(List<vec3>& vertices, List<vec3>& tmpVertices, List<vec2>& texCoords, List<vec2>& tmpTexCoords, List<vec3>& normals, List<vec3>& tmpNormals, List<uint32>& indices, List<Face<3>> faces);
public:

	static inline Model* CreatePlane(const vec2& size) { return CreatePlane(size.x, size.y); }
	static inline Model* CreateCube(const vec3& size) { return CreateCube(size.x, size.y, size.z); }

	static Model* CreatePlane(float32 width, float32 height);
	static Model* CreateCube(float32 width, float32 height, float32 depth);

	static Model* LoadFromFile(const String& filename);
};
