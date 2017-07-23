#include "debug.h"

#include <graphics/render/mesh/mesh.h>
#include <graphics/render/mesh/meshfactory.h>

const char* showTexVertex =
#include <graphics/shader/shaders/debug/showTextureVertex.hlsl>
;

const char* showTexPixel =
#include <graphics/shader/shaders/debug/showTexturePixel.hlsl>
;

namespace FD {

void Debug::ShowTexture(Texture* tex) {
	Shader* shader = new Shader(showTexVertex, showTexPixel, true);
	shader->ShaderGenComplete();

	BufferLayout layout;
	layout.Push<vec3>("POSITION"); 
	layout.Push<vec3>("NORMAL");
	layout.Push<vec2>("TEXCOORD");


	layout.CreateInputLayout(shader);
	Material* mat = new Material(shader);

	mat->SetTexture("tex", tex);
	Mesh* mesh = MeshFactory::CreatePlane(2, 2, mat);

	mesh->Render();

	delete mesh;
	delete shader;
}

}