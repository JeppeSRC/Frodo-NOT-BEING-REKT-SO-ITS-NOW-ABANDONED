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
	Window w("Title", 600, 400);
	w.SetVisible(true);
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

	shader.Bind();
	layout.Bind();

	v.Bind();
	i.Bind();

	D3DContext::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	

	while (w.IsOpen()) {
		w.Clear();

		D3DContext::GetDeviceContext()->DrawIndexed(i.GetCount(), 0, 0);

		w.SwapBuffers();
	}

	return 0;
}