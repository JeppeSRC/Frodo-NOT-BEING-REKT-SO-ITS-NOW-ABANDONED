#include <stdio.h>
#include <util/list.h>
#include <util/map.h>
#include <util/string.h>
#include <core/log.h>
#include <core/window.h>
#include <graphics/buffer/bufferlayout.h>
#include <graphics/buffer/vertexbuffer.h>
#include <graphics/buffer/indexbuffer.h>
#include <graphics/texture/texture2d.h>
#include <graphics/shader/shader.h>

#include <math/math.h>

#include <ctime>

#define PRINT(str) FD_INFO("%s", *str)

int main() {
	FD_INFO("INFO");
	FD_DEBUG("DEBUG");
	FD_WARNING("WARNING");
	FD_FATAL("FATAL");
	Window w("Title", 1000, 600);
//	w.SetVisible(true);
	w.SetVSync(1);

	struct Vertex {
		vec4 position;
		vec4 color;
	};
	
	unsigned int indices[] = {0, 1, 2};

	Vertex a[3]{
		{vec4(0, 1, 0, 1), vec4(1, 0, 0, 1)},
		{vec4(1, -1, 0, 1), vec4(0, 1, 0, 1)},
		{vec4(-1, -1, 0, 1), vec4(0, 0, 1, 1)}
	};

	a[0].position = (mat4::Translate(vec3(0, 0, -2)) * mat4::Rotate(vec3(0, 0, 45))) * vec4(0, 1, 0, 1);
	a[1].position = (mat4::Translate(vec3(0, 0, -2)) * mat4::Rotate(vec3(0, 0, 45))) * vec4(1, -1, 0, 1);
	a[2].position = (mat4::Translate(vec3(0, 0, -2)) * mat4::Rotate(vec3(0, 0, 45))) * vec4(-1, -1, 0, 1);

	VertexBuffer v(&a, sizeof(a), sizeof(Vertex));
	IndexBuffer i(indices, 3);

	Shader shader("res/vertex.hlsl", "res/pixel.hlsl");
	BufferLayout layout;

	layout.Push<vec4>("POSITION");
	layout.Push<vec4>("COLOR");

	layout.CreateInputLayout(&shader);

	shader.Bind();
	layout.Bind();

	v.Bind();
	i.Bind();

	D3DContext::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	
	
	float temp = 0;
	struct Mod {
		mat4 projection;
		mat4 model;
	} mod;

	mod.projection = mat4::Perspective(70.0f, w.GetAspectRatio(), 0.001f, 100.0f);
//	mod.projection = mat4::Orthographic(-1, 1, 1, -1, 0.001f, 100.0f);
	mod.model = mat4::Identity();
//	mod.projection = mat4::Identity();
	
	while (w.IsOpen()) {
		temp += 0.1f;
		w.Clear();

		//mod.model = mat4::Translate(vec3(0, 0, -2)) * mat4::Rotate(vec3(0, 0, temp));

		shader.SetVSConstantBuffer(0, &mod);


		D3DContext::GetDeviceContext()->DrawIndexed(i.GetCount(), 0, 0);

		w.SwapBuffers();
	}

	return 0;
}