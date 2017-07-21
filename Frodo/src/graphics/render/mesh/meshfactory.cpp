#include "meshfactory.h"
#include <util/list.h>
#include <util/fileutils.h>
#include <core/log.h>
#include <util/vfs/vfs.h>

namespace FD {

struct Vertex {
	vec3 position;
	vec3 normal;
	vec2 texCoord;
};

struct VertexT {
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	vec3 tangent;
};

Mesh* MeshFactory::CreatePlane(float32 width, float32 height, Material* material) {



	float32 w = width / 2.0f;
	float32 h = height / 2.0f;

	vec3 vertices[4]{
		{-w, h, 0},
		{w, h, 0},
		{w, -h, 0},
		{-w, -h, 0}
	};

	vec2 tex[4]{
		{0, 0},
		{1, 0},
		{1, 1},
		{0, 1 }
	};

	vec3 normals[4]{
		{0, 0, -1},
		{0, 0, -1},
		{0, 0, -1},
		{0, 0, -1}
	};

	Vertex v[4];

	for (int32 i = 0; i < 4; i++) {
		v[i].position = vertices[i];
		v[i].texCoord = tex[i];
		v[i].normal = normals[i];
	}

	VertexBuffer* vBuffer = new VertexBuffer(v, sizeof(v), sizeof(Vertex));

	uint32 indices[6]{
		0, 1, 2, 2, 3, 0
	};

	IndexBuffer* iBuffer = new IndexBuffer(indices, 6);

	Mesh* mesh = new Mesh(vBuffer, iBuffer, material);

	return mesh;
}

Mesh* MeshFactory::CreateCube(float32 width, float32 height, float32 depth, Material* material) {

	float32 w = width / 2.0f;
	float32 h = height / 2.0f;
	float32 d = depth / 2.0f;

	vec3 vertices[24]{

		//front
		{-w, h, -d},
		{w, h, -d},
		{w, -h, -d},
		{-w, -h, -d},

		//back
		{w, h, d},
		{-w, h, d},
		{-w, -h, d},
		{w, -h, d},


		//right
		{w, h, -d},
		{w, h, d},
		{w, -h, d},
		{w, -h, -d},

		//left
		{-w, h, d},
		{-w, h, -d},
		{-w, -h, -d},
		{-w, -h, d},

		//top
		{-w, h, d},
		{w, h, d},
		{w, h, -d},
		{-w, h, -d},

		//bottom
		{-w, -h, -d},
		{w, -h, -d},
		{w, -h, d},
		{-w, -h, d}
	};

	vec2 tex[24]{
		{0, 0},
		{1, 0},
		{1, 1},
		{0, 1},

		{0, 0},
		{1, 0},
		{1, 1},
		{0, 1},

		{0, 0},
		{1, 0},
		{1, 1},
		{0, 1},

		{0, 0},
		{1, 0},
		{1, 1},
		{0, 1},

		{0, 0},
		{1, 0},
		{1, 1},
		{0, 1},

		{0, 0},
		{1, 0},
		{1, 1},
		{0, 1}
	};

	vec3 normals[24]{

		//front
		{0, 0, -1},
		{0, 0, -1},
		{0, 0, -1},
		{0, 0, -1},


		//back
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},

		//right
		{1, 0, 0},
		{1, 0, 0},
		{1, 0, 0},
		{1, 0, 0},

		//left
		{-1, 0, 0},
		{-1, 0, 0},
		{-1, 0, 0},
		{-1, 0, 0},

		//top
		{0, 1, 0},
		{0, 1, 0},
		{0, 1, 0},
		{0, 1, 0},

		//bottom
		{0, -1, 0},
		{0, -1, 0},
		{0, -1, 0},
		{0, -1, 0}
	};

	Vertex v[24];

	for (int32 i = 0; i < 24; i++) {
		v[i].position = vertices[i];
		v[i].texCoord = tex[i];
		v[i].normal = normals[i];
	}

	VertexBuffer* vBuffer = new VertexBuffer(v, sizeof(v), sizeof(Vertex));

	uint32 indices[36]{
		//front
		0, 1, 2, 2, 3, 0,

		//back
		4, 5, 6, 6, 7, 4,

		//right
		8, 9, 10, 10, 11, 8,

		//left
		12, 13, 14, 14, 15, 12,

		//top
		16, 17, 18, 18, 19, 16,

		//bottom
		20, 21, 22, 22, 23, 20,
	};

	IndexBuffer* iBuffer = new IndexBuffer(indices, 36);

	Mesh* mesh = new Mesh(vBuffer, iBuffer, material);

	return mesh;
}

Mesh* MeshFactory::LoadFromFile(const String& filename, Material* material, bool generateTangents) {

	if (filename.EndsWith(".obj")) {
		List<vec3> vertices, normals, tangents;
		List<vec2> texCoords;
		List<uint32> indices;

		FD_DEBUG("[MeshFactory] Loading \"%s\"", *filename);
		FD_DEBUG("[MeshFactory] Loading text file");
		String text = VFS::Get()->ReadTextFile(filename);

		//TODO: better implementation
		if (!generateTangents) {
			ParseOBJ(text, vertices, texCoords, normals, indices);

			uint_t vertNum = vertices.GetSize();

			VertexT* vert = new VertexT[vertNum];

			FD_DEBUG("[MeshFactory] Moving data");
			for (uint_t i = 0; i < vertNum; i++) {
				VertexT& v = vert[i];
				v.position = vertices[i];
				v.texCoord = texCoords[i];
				v.normal = normals[i];
				v.tangent = vec3(0, 0, 0);
			}

			FD_DEBUG("[MeshFactory] Loading data");
			VertexBuffer* vbo = new VertexBuffer(vert, vertNum * sizeof(VertexT), sizeof(VertexT));
			IndexBuffer* ibo = new IndexBuffer(indices.GetData(), (uint32)indices.GetSize());

			FD_DEBUG("[MeshFactory] Loading complete!\n");
			return new Mesh(vbo, ibo, material);
		} else {
			ParseOBJT(text, vertices, texCoords, normals, tangents, indices);

			uint_t vertNum = vertices.GetSize();

			VertexT* vert = new VertexT[vertNum];

			FD_DEBUG("[MeshFactory] Moving data");
			for (uint_t i = 0; i < vertNum; i++) {
				VertexT& v = vert[i];
				v.position = vertices[i];
				v.texCoord = texCoords[i];
				v.normal = normals[i];
				v.tangent = tangents[i];
			}

			FD_DEBUG("[MeshFactory] Loading data");
			VertexBuffer* vbo = new VertexBuffer(vert, vertNum * sizeof(VertexT), sizeof(VertexT));
			IndexBuffer* ibo = new IndexBuffer(indices.GetData(), (uint32)indices.GetSize());

			FD_DEBUG("[MeshFactory] Loading complete!\n");
			return new Mesh(vbo, ibo, material);
		}

	} else {
		FD_WARNING("\"%s\" Only supports obj files ATM!", __FUNCSIG__);
	}

	return new Mesh(nullptr, nullptr, nullptr);

}

void MeshFactory::ParseOBJ(const String obj, List<vec3>& vertices, List<vec2>& texCoords, List<vec3>& normals, List<uint32>& indices) {

	List<Face<3>> faces;
	List<String*> lines = obj.Split('\n');
	List<vec3> tmpVertices, tmpNormals;
	List<vec2> tmpTexCoords;



	uint_t numLines = lines.GetSize();
	FD_DEBUG("[MeshFactory] Parsing text");

	for (int32 i = 0; i < numLines; i++) {
		String line = lines[i];

		if (line.StartsWith("v ")) {
			vec3 vert;
			sscanf(*line, "v %f %f %f", &vert.x, &vert.y, &vert.z);
			tmpVertices.Push_back(vert);
		} else if (line.StartsWith("vt ")) {
			vec2 tex;
			sscanf(*line, "vt %f %f", &tex.x, &tex.y);
			tmpTexCoords.Push_back(tex);
		} else if (line.StartsWith("vn ")) {
			vec3 norm;
			sscanf(*line, "vn %f %f %f", &norm.x, &norm.y, &norm.z);
			tmpNormals.Push_back(norm);
		} else if (line.StartsWith("f ")) {
			Face<3> face;
			sscanf(*line, "f %u/%u/%u %u/%u/%u %u/%u/%u", &face[0].vertex, &face[0].texCoord, &face[0].normal, &face[1].vertex, &face[1].texCoord, &face[1].normal, &face[2].vertex, &face[2].texCoord, &face[2].normal);
			faces.Push_back(face);
		}
	}

	FD_DEBUG("[MeshFactory] Found %u vertices %u normals %u uvs %u faces", tmpVertices.GetSize(), tmpNormals.GetSize(), tmpTexCoords.GetSize(), faces.GetSize());
	FD_DEBUG("[MeshFactory] Allocating space");
	texCoords.Resize(faces.GetSize() * 3);
	normals.Resize(faces.GetSize() * 3);
	vertices.Resize(faces.GetSize() * 3);

	FD_DEBUG("[MeshFactory] Making faces");
	MakeFacesOBJ(vertices, tmpVertices, texCoords, tmpTexCoords, normals, tmpNormals, indices, faces);

	lines.Free();
}

void MeshFactory::ParseOBJT(const String obj, List<vec3>& vertices, List<vec2>& texCoords, List<vec3>& normals, List<vec3>& tangents, List<uint32>& indices) {

	List<Face<3>> faces;
	List<String*> lines = obj.Split('\n');
	List<vec3> tmpVertices, tmpNormals;
	List<vec2> tmpTexCoords;


	uint_t numLines = lines.GetSize();
	FD_DEBUG("[MeshFactory] Parsing text");

	for (int32 i = 0; i < numLines; i++) {
		String line = lines[i];

		if (line.StartsWith("v ")) {
			vec3 vert;
			sscanf(*line, "v %f %f %f", &vert.x, &vert.y, &vert.z);
			tmpVertices.Push_back(vert);
		} else if (line.StartsWith("vt ")) {
			vec2 tex;
			sscanf(*line, "vt %f %f", &tex.x, &tex.y);
			tmpTexCoords.Push_back(tex);
		} else if (line.StartsWith("vn ")) {
			vec3 norm;
			sscanf(*line, "vn %f %f %f", &norm.x, &norm.y, &norm.z);
			tmpNormals.Push_back(norm);
		} else if (line.StartsWith("f ")) {
			Face<3> face;
			sscanf(*line, "f %u/%u/%u %u/%u/%u %u/%u/%u", &face[0].vertex, &face[0].texCoord, &face[0].normal, &face[1].vertex, &face[1].texCoord, &face[1].normal, &face[2].vertex, &face[2].texCoord, &face[2].normal);
			faces.Push_back(face);
		}
	}

	FD_DEBUG("[MeshFactory] Found %u vertices %u normals %u uvs %u faces", tmpVertices.GetSize(), tmpNormals.GetSize(), tmpTexCoords.GetSize(), faces.GetSize());
	FD_DEBUG("[MeshFactory] Allocating space");
	texCoords.Resize(faces.GetSize() * 3);
	normals.Resize(faces.GetSize() * 3);
	vertices.Resize(faces.GetSize() * 3);
	tangents.Resize(faces.GetSize() * 3);

	FD_DEBUG("[MeshFactory] Making faces");
	MakeFacesOBJT(vertices, tmpVertices, texCoords, tmpTexCoords, normals, tmpNormals, tangents, indices, faces);

	lines.Free();
}


void MeshFactory::MakeFacesOBJ(List<vec3>& vertices, List<vec3>& tmpVertices, List<vec2>& texCoords, List<vec2>& tmpTexCoords, List<vec3>& normals, List<vec3>& tmpNormals, List<uint32>& indices, List<Face<3>> faces) {

	uint_t numFaces = faces.GetSize();

	indices.Reserve(numFaces * 3);
#pragma omp for
	for (int32 i = 0; i < numFaces; i++) {
		Face<3> face = faces[i];
		for (int32 j = 0; j < 3; j++) {
			uint32 index = i * 3 + j;//indices.GetSize();

			indices.Push_back(index);
			vertices[index] = tmpVertices[face[j].vertex - 1];
			texCoords[index] = tmpTexCoords[face[j].texCoord - 1];
			normals[index] = tmpNormals[face[j].normal - 1];
		}
	}
}



void MeshFactory::MakeFacesOBJT(List<vec3>& vertices, List<vec3>& tmpVertices, List<vec2>& texCoords, List<vec2>& tmpTexCoords, List<vec3>& normals, List<vec3>& tmpNormals, List<vec3>& tangents, List<uint32>& indices, List<Face<3>> faces) {

	uint_t numFaces = faces.GetSize();

	struct V {
		uint32 v;
		uint32 t;
		uint32 n;
	};

	Map<V, uint32> vertexCache(numFaces * 3);

	indices.Reserve(numFaces * 3);

	auto findSimilar = [&](uint32 v, uint32 t, uint32 n) -> uint32 {
		uint_t size = vertexCache.GetItems();

		FD_MAP_PAIR<V, uint32> pair;

		for (uint_t i = 0; i < size; i++) {
			pair = vertexCache.GetPair(i);

			if (v == pair.key.v && t == pair.key.t && n == pair.key.n) {
				return pair.data;
			}
		}

		return ~0;
	};

	uint32 index = 0;

	for (int32 i = 0; i < numFaces; i++) {
		Face<3> face = faces[i];
		
		//vertex 0
		//uint32 index0 = i * 3 + 0;
		uint32 vIndex0 = face[0].vertex - 1;
		uint32 tIndex0 = face[0].texCoord - 1;
		uint32 nIndex0 = face[0].normal - 1;

		vec3 v0 = tmpVertices[vIndex0];
		vec2 t0 = tmpTexCoords[tIndex0];
		

		//vertex 1
	//	uint32 index1 = i * 3 + 1;
		uint32 vIndex1 = face[1].vertex - 1;
		uint32 tIndex1 = face[1].texCoord - 1;
		uint32 nIndex1 = face[1].normal - 1;

		vec3 v1 = tmpVertices[vIndex1];
		vec2 t1 = tmpTexCoords[tIndex1];


		//vertex 2
		//uint32 index2 = i * 3 + 2;
		uint32 vIndex2 = face[2].vertex - 1;
		uint32 tIndex2 = face[2].texCoord - 1;
		uint32 nIndex2 = face[2].normal - 1;

		vec3 v2 = tmpVertices[vIndex2];
		vec2 t2 = tmpTexCoords[tIndex2];

		//tangent calculation
		vec3 e0 = v1 - v0;
		vec3 e1 = v2 - v0;

		vec2 dT0 = t1 - t0;
		vec2 dT1 = t2 - t0;

		float32 f = 1.0f / (dT0.x * dT1.y - dT0.y * dT1.x);

		float32 tX = f * (dT1.y * e0.x - dT0.y * e1.x);
		float32 tY = f * (dT1.y * e0.y - dT0.y * e1.y);
		float32 tZ = f * (dT1.y * e0.z - dT0.y * e1.z);

		vec3 tangent = vec3(tX, tY, tZ).Normalize();

		uint32 newIndex = 0;

		//vertex 0
		if ((newIndex = findSimilar(vIndex0, tIndex0, nIndex0)) != ~0) {
			indices.Push_back(newIndex);

			tangents[newIndex] += tangent;
		} else {
			indices.Push_back(index);

			vertices[index] = v0;
			texCoords[index] = t0;
			normals[index] = tmpNormals[nIndex0];
			tangents[index] = tangent;

			vertexCache.Add({ vIndex0, tIndex0, nIndex0 }, index);
			index++;
		}

		//vertex 1
		if ((newIndex = findSimilar(vIndex1, tIndex1, nIndex1)) != ~0) {
			indices.Push_back(newIndex);

			tangents[newIndex] += tangent;
		} else {
			indices.Push_back(index);

			vertices[index] = v1;
			texCoords[index] = t1;
			normals[index] = tmpNormals[nIndex1];
			tangents[index] = tangent;

			vertexCache.Add({ vIndex1, tIndex1, nIndex1 }, index);
			index++;
		}

		//vertex 2
		if ((newIndex = findSimilar(vIndex2, tIndex2, nIndex2)) != ~0) {
			indices.Push_back(newIndex);

			tangents[newIndex] += tangent;
		} else {
			indices.Push_back(index);

			vertices[index] = v2;
			texCoords[index] = t2;
			normals[index] = tmpNormals[nIndex2];
			tangents[index] = tangent;

			vertexCache.Add({ vIndex2, tIndex2, nIndex2 }, index);
			index++;
		}
	}
}

}