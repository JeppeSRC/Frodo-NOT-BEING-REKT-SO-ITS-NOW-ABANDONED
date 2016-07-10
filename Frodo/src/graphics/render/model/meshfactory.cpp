#include "meshfactory.h"

Model* MeshFactory::CreatePlane(float width, float height) {

	struct Vertex {
		vec3 position;
		vec2 texCoords;
		vec3 normals;
	};

	float w = width / 2;
	float h = height / 2;

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

	vec3 normals[12]{
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1 }
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