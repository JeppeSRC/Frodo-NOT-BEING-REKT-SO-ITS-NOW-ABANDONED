#include "meshfactory.h"

Model* MeshFactory::CreatePlane(float width, float height) {

	struct Vertex {
		vec3 position;
		vec2 texCoords;
		vec3 normals;
	};

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

	struct Vertex {
		vec3 position;
		vec2 texCoords;
		vec3 normals;
	};

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