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

int main() {
	Window w("Title", 600, 400);
	w.SetVisible(true);
	w.SetVSync(1);

	float a[]{
		0, 1, 0,
		1, -1, 0,
		-1, -1, 0
	};

	unsigned int indices[] = {0, 1, 2};

	FDVertexBuffer v(&a, sizeof(a), 12);
	FDIndexBuffer i(indices, 3);

	vec4 pix[4]{
		{1, 0, 1, 1},
		{1, 0, 1, 1},
		{1, 0, 1, 1},
		{1, 0, 1, 1}
	};

	FDTexture2D texture(pix, 2, 2, FD_TEXTURE2D_FORMAT_FLOAT_32_32_32_32);

	FDShader shader("res/vertex.hlsl", "res/pixel.hlsl");
	FDBufferLayout layout;

	layout.Push<vec3>("POSITION");

	layout.CreateInputLayout(&shader);

	layout.Bind();
	shader.Bind();

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