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

#define PRINT(str) _INFO("%s", *str)

int main() {
	Window w("Title", 1000, 600);
//	w.SetVisible(true);
	w.SetVSync(1);

	struct Vertex {
		vec4 position;
		vec4 color;
	};
	
	unsigned int indices[] = {0, 1, 2};

	Vertex a[4]{
		{vec4(0, 1, 0, 1), vec4(1, 0, 0, 1)},
		{vec4(1, -1, 0, 1), vec4(0, 1, 0, 1)},
		{vec4(-1, -1, 0, 1), vec4(0, 0, 1, 1)}
	};

	VertexBuffer v(&a, sizeof(a), sizeof(Vertex));
	IndexBuffer i(indices, 3);

	Shader shader("res/vertex.hlsl", "res/pixel.hlsl");
	BufferLayout layout;

	layout.Push<vec4>("POSITION");
	layout.Push<vec4>("COLOR");

	layout.CreateInputLayout(&shader);

	mat4 proj = mat4::Translate(vec3(0.5, 0, 0));


	shader.Bind();
	layout.Bind();

	v.Bind();
	i.Bind();
	
	D3DContext::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	
	
	float temp = 0;

	struct Mod {
		mat4 translation;
		mat4 roation;
	} mod;



	while (w.IsOpen()) {
		temp += 0.01f;
		w.Clear();

		mod.translation = mat4::Translate(vec3(cosf(temp), 0, 0));
		mod.roation = mat4::Rotate(vec3(0, 0, sinf(temp) * 90));

		shader.SetVSConstantBuffer(0, &mod);


		D3DContext::GetDeviceContext()->DrawIndexed(i.GetCount(), 0, 0);

		w.SwapBuffers();
	}

	return 0;
}