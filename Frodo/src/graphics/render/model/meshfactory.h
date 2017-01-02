#pragma once

#include "model.h"
#include <util/list.h>

class FDAPI MeshFactory {
private:

	template<unsigned int N>
	struct Face {
		struct Vertex {
			unsigned int vertex;
			unsigned int texCoord;
			unsigned int normal;
		} v[N];

		inline Vertex& operator[](size_t index) { return v[index]; }
	};

	static void ParseOBJ(const String obj, List<vec3>& vertices, List<vec2>& texCoords, List<vec3>& normals, List<unsigned int>& indices);
	static void MakeFacesOBJ(List<vec3>& vertices, List<vec3>& tmpVertices, List<vec2>& texCoords, List<vec2>& tmpTexCoords, List<vec3>& normals, List<vec3>& tmpNormals, List<unsigned int>& indices, List<Face<3>> faces);
public:

	static inline Model* CreatePlane(const vec2& size) { return CreatePlane(size.x, size.y); }
	static inline Model* CreateCube(const vec3& size) { return CreateCube(size.x, size.y, size.z); }

	static Model* CreatePlane(float width, float height);
	static Model* CreateCube(float width, float height, float depth);

	static Model* LoadFromFile(const String& filename);
};
