#include "meshfactory.h"
#include <util/list.h>
#include <util/fileutils.h>
#include <core/log.h>
#include <util/vfs/vfs.h>

struct Vertex {
	vec3 position;
	vec2 texCoords;
	vec3 normals;
};

Model* MeshFactory::CreatePlane(float width, float height) {

	

	float w = width / 2.0f;
	float h = height / 2.0f;

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

	for (int i = 0; i < 4; i++) {
		v[i].position = vertices[i];
		v[i].texCoords = tex[i];
		v[i].normals = normals[i];
	}

	VertexBuffer* vBuffer = new VertexBuffer(v, sizeof(v), sizeof(Vertex));

	unsigned int indices[6]{
		0, 1, 2, 2, 3, 0
	};

	IndexBuffer* iBuffer = new IndexBuffer(indices, 6);

	Model* model = new Model(vBuffer, iBuffer);

	return model;
}

Model* MeshFactory::CreateCube(float width, float height, float depth) {

	float w = width / 2.0f;
	float h = height / 2.0f;
	float d = depth / 2.0f;

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

	for (int i = 0; i < 24; i++) {
		v[i].position = vertices[i];
		v[i].texCoords = tex[i];
		v[i].normals = normals[i];
	}

	VertexBuffer* vBuffer = new VertexBuffer(v, sizeof(v), sizeof(Vertex));

	unsigned int indices[36]{
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

	Model* model = new Model(vBuffer, iBuffer);

	return model;
}

Model* MeshFactory::LoadFromFile(const String& filename) {

	if (filename.EndsWith(".obj")) {
		List<vec3> vertices, normals;
		List<vec2> texCoords;
		List<unsigned int> indices;

		String text = VFS::Get()->ReadTextFile(filename);

		ParseOBJ(text, vertices, texCoords, normals, indices);

		size_t vertNum = vertices.GetSize();

		Vertex* vert = new Vertex[vertNum];

		for (size_t i = 0; i < vertNum; i++) {
			Vertex& v = vert[i];
			v.position = vertices[i];
			v.texCoords = texCoords[i];
			v.normals = normals[i];
		}

		VertexBuffer* vbo = new VertexBuffer(vert, vertNum * sizeof(Vertex), sizeof(Vertex));
		IndexBuffer* ibo = new IndexBuffer(indices.GetData(), indices.GetSize());

		return new Model(vbo, ibo);

	} else {
		FD_WARNING("\"%s\" Only supports obj files ATM!", __FUNCSIG__);
	}

	return new Model;

}

void MeshFactory::ParseOBJ(const String obj, List<vec3>& vertices, List<vec2>& texCoords, List<vec3>& normals, List<unsigned int>& indices) {

	List<Face<3>> faces;
	List<String*> lines = obj.Split('\n');


	size_t numLines = lines.GetSize();

	for (size_t i = 0; i < numLines; i++) {
		String line = lines[i];

		if (line.StartsWith("v ")) {
			vec3 vert;
			sscanf(*line, "v %f %f %f", &vert.x, &vert.y, &vert.z);
			vertices.Push_back(vert);
		} else if (line.StartsWith("vt ")) {
			vec2 tex;
			sscanf(*line, "vt %f %f", &tex.x, &tex.y);
			texCoords.Push_back(tex);
		} else if (line.StartsWith("vn ")) {
			vec3 norm;
			sscanf(*line, "vn %f %f %f", &norm.x, &norm.y, &norm.z);
			normals.Push_back(norm);
		} else if (line.StartsWith("f ")) {
			Face<3> face;
			sscanf(*line, "f %u/%u/%u %u/%u/%u %u/%u/%u", &face[0].vertex, &face[0].texCoord, &face[0].normal, &face[1].vertex, &face[1].texCoord, &face[1].normal, &face[2].vertex, &face[2].texCoord, &face[2].normal);
			faces.Push_back(face);
		}
	}

	texCoords.Resize(vertices.GetSize());
	normals.Resize(vertices.GetSize());

	MakeFacesOBJ(texCoords, normals, indices, faces);
}

void MeshFactory::MakeFacesOBJ(List<vec2>& texCoords, List<vec3>& normals, List<unsigned int>& indices, List<Face<3>> faces) {

	List<vec2> tmpTexCoords = texCoords;
	List<vec3> tmpNormals = normals;


	size_t numFaces = faces.GetSize();

	indices.Resize(numFaces * 3);

	for (size_t i = 0; i < numFaces; i++) {
		Face<3> face = faces[i];
		for (size_t j = 0; j < 3; j++) {
			unsigned int vertex = face[j].vertex - 1;
			indices[i * 3 + j] = vertex;
			texCoords[vertex] = tmpTexCoords[face[j].texCoord-1];
			normals[vertex] = tmpNormals[face[j].normal-1];
		}
	}
}
