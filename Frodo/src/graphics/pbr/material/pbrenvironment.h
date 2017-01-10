#pragma once

#include <graphics/buffer/vertexbuffer.h>
#include <graphics/buffer/indexbuffer.h>
#include <graphics/texture/texturecube.h>
#include <math/vec3.h>

class FDAPI PBREnvironment {
private:
	VertexBuffer* vBuffer;
	IndexBuffer* iBuffer;

public:
	TextureCube* cubeMap;

	PBREnvironment(vec3 size, TextureCube* cubeMap);
	~PBREnvironment();

	void Bind();
};
