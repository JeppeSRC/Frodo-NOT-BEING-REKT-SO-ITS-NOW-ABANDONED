#pragma once

#include "mesh.h"
#include <graphics/render/material/material.h>
#include <util/list.h>

namespace FD {

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
	static void ParseOBJT(const String obj, List<vec3>& vertices, List<vec2>& texCoords, List<vec3>& normals, List<vec3>& tangents, List<uint32>& indices);
	static void MakeFacesOBJ(List<vec3>& vertices, List<vec3>& tmpVertices, List<vec2>& texCoords, List<vec2>& tmpTexCoords, List<vec3>& normals, List<vec3>& tmpNormals, List<uint32>& indices, List<Face<3>> faces);
	static void MakeFacesOBJT(List<vec3>& vertices, List<vec3>& tmpVertices, List<vec2>& texCoords, List<vec2>& tmpTexCoords, List<vec3>& normals, List<vec3>& tmpNormals, List<vec3>& tangents, List<uint32>& indices, List<Face<3>> faces);
public:

	static inline Mesh* CreatePlane(const vec2& size, MaterialInstance* material) { return CreatePlane(size.x, size.y, material); }
	static inline Mesh* CreateCube(const vec3& size,  MaterialInstance* material) { return CreateCube(size.x, size.y, size.z, material); }

	static Mesh* CreatePlane(float32 width, float32 height, MaterialInstance* material);
	static Mesh* CreateCube(float32 width, float32 height, float32 depth, MaterialInstance* material);

	static Mesh* LoadFromFile(const String& filename, MaterialInstance* material, bool generateTangents);
};

}