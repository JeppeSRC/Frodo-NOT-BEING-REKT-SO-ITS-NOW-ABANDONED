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
		
		FD_DEBUG("[MeshFactory] Loading \"%s\"", *filename);
		FD_DEBUG("[MeshFactory] Loading text file");
		String text = VFS::Get()->ReadTextFile(filename);

		ParseOBJ(text, vertices, texCoords, normals, indices);

		size_t vertNum = vertices.GetSize();

		Vertex* vert = new Vertex[vertNum];

		FD_DEBUG("[MeshFactory] Moving data");
		for (size_t i = 0; i < vertNum; i++) {
			Vertex& v = vert[i];
			v.position = vertices[i];
			v.texCoords = texCoords[i];
			v.normals = normals[i];
		}

		FD_DEBUG("[MeshFactory] Loading data");
		VertexBuffer* vbo = new VertexBuffer(vert, vertNum * sizeof(Vertex), sizeof(Vertex));
		IndexBuffer* ibo = new IndexBuffer(indices.GetData(), (unsigned int)indices.GetSize());

		FD_DEBUG("[MeshFactory] Loading complete!\n");
		return new Model(vbo, ibo);

	} else {
		FD_WARNING("\"%s\" Only supports obj files ATM!", __FUNCSIG__);
	}

	return new Model;

}

void MeshFactory::ParseOBJ(const String obj, List<vec3>& vertices, List<vec2>& texCoords, List<vec3>& normals, List<unsigned int>& indices) {

	List<Face<3>> faces;
	List<String*> lines = obj.Split('\n');
	List<vec3> tmpVertices, tmpNormals;
	List<vec2> tmpTexCoords;


	
	size_t numLines = lines.GetSize();
	FD_DEBUG("[MeshFactory] Parsing text");
	#pragma omp for
	for (int i = 0; i < numLines; i++) {
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
}

void MeshFactory::MakeFacesOBJ(List<vec3>& vertices, List<vec3>& tmpVertices, List<vec2>& texCoords, List<vec2>& tmpTexCoords, List<vec3>& normals, List<vec3>& tmpNormals, List<unsigned int>& indices, List<Face<3>> faces) {

	size_t numFaces = faces.GetSize();

	indices.Reserve(numFaces * 3);
	#pragma omp for
	for (int i = 0; i < numFaces; i++) {
		Face<3> face = faces[i];
		for (int j = 0; j < 3; j++) {
			unsigned int index = i * 3 + j;//indices.GetSize();

			indices.Push_back(index);
			vertices[index] = tmpVertices[face[j].vertex - 1];
			texCoords[index] = tmpTexCoords[face[j].texCoord-1];
			normals[index] = tmpNormals[face[j].normal-1];
		}
	}
}
