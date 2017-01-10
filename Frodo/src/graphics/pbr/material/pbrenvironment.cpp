#include "pbrenvironment.h"

struct Vertex {
	vec3 position;
	float pad0;
	vec3 texCoord;
	float pad1;
};

PBREnvironment::PBREnvironment(vec3 size, TextureCube* cubeMap) : cubeMap(cubeMap) {

	float x = size.x / 2.0f;
	float y = size.y / 2.0f;
	float z = size.z / 2.0f;

	Vertex vertices[8]{
		{ vec3(-x,  y, -z), 0, vec3(-1,  1, -1), 0 },
		{ vec3( x,  y, -z), 0, vec3( 1,  1, -1), 0 },
		{ vec3( x, -y, -z), 0, vec3( 1, -1, -1), 0 },
		{ vec3(-x, -y, -z), 0, vec3(-1, -1, -1), 0 },
							
		{ vec3(-x,  y,  z), 0, vec3(-1,  1,  1), 0 },
		{ vec3( x,  y,  z), 0, vec3( 1,  1,  1), 0 },
		{ vec3( x, -y,  z), 0, vec3( 1, -1,  1), 0 },
		{ vec3(-x, -y,  z), 0, vec3(-1, -1,  1), 0 }
	};

	unsigned int indices[36]{
		//front
		2, 1, 0, 0, 3, 2,
		//back
		4, 5, 6, 6, 7, 4,
		//top
		0, 1, 4, 4, 1, 5,
		//bottom
		7, 6, 3, 3, 6, 2,
		//left
		0, 4, 7, 7, 3, 0,
		//righ
		5, 1, 2, 2, 6, 5
	};

	vBuffer = new VertexBuffer(vertices, sizeof(vertices), sizeof(Vertex));
	iBuffer = new IndexBuffer(indices, 36);
}

PBREnvironment::~PBREnvironment() {
	delete vBuffer;
	delete iBuffer;
}

void PBREnvironment::Bind() {
	vBuffer->Bind();
	iBuffer->Bind();
}