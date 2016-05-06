#include <stdio.h>
#include "game.h"
#define PRINT(str) FD_INFO("%s", *str)


int main() {
	/*printf("%08x\n", 12);
	Window w("Title", 1000, 600);

	w.SetVSync(1);

	struct Vertex {
		vec4 position;
		vec2 color;
	};
	
	unsigned int indices[] = {0, 1, 2};
	unsigned int indices2[] = {0, 1, 2, 2, 3, 0};

	Vertex a[3]{
		{vec4(0, 1, 0, 1), vec2(0.5, 0)},
		{vec4(1, -1, 0, 1), vec2(1, 1)},
		{vec4(-1, -1, 0, 1), vec2(0, 1)}
	};

	Vertex b[4]{
		{vec4(-1, 1, 0, 1), vec2(0, 0)},
		{vec4(1, 1, 0, 1), vec2(1, 0)},
		{vec4(1, -1, 0, 1), vec2(1, 1)},
		{vec4(-1, -1, 0, 1), vec2(0, 1)}
	};

/*	a[0].position = (mat4::Translate(vec3(0, 0, -2)) * mat4::Rotate(vec3(0, 0, 45))) * vec4(0, 1, 0, 1);
	a[1].position = (mat4::Translate(vec3(0, 0, -2)) * mat4::Rotate(vec3(0, 0, 45))) * vec4(1, -1, 0, 1);
	a[2].position = (mat4::Translate(vec3(0, 0, -2)) * mat4::Rotate(vec3(0, 0, 45))) * vec4(-1, -1, 0, 1);
	
	VertexBuffer v(&a, sizeof(a), sizeof(Vertex));
	IndexBuffer i(indices, 3);

	VertexBuffer v2(&b, sizeof(b), sizeof(Vertex));
	IndexBuffer i2(indices2, 6);

	Texture2D tex("./res/mountains.jpg");
	Framebuffer2D framebuffer(5000 * w.GetAspectRatio(), 5000, FD_TEXTURE_FORMAT_FLOAT_32_32_32_32);

	Shader shader("res/vertex.hlsl", "res/pixel.hlsl");
	Shader shader2("res/displayVertex.hlsl", "res/pixel.hlsl");

	BufferLayout layout;

	layout.Push<vec4>("POSITION");
	layout.Push<vec2>("TEXCOORDS");

	layout.CreateInputLayout(&shader);

	layout.Bind();


	D3DContext::SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	
	
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
		framebuffer.BindAsRenderTarget();
		D3DContext::Clear();

		mod.model = mat4::Translate(vec3(0, 0, -2)) * mat4::Rotate(vec3(0, 0, temp));

		shader.Bind();
		shader.SetVSConstantBuffer(0, &mod);
		shader.SetTexture(0, &tex);


		v.Bind();
		i.Bind();

		D3DContext::GetDeviceContext()->DrawIndexed(i.GetCount(), 0, 0);
		D3DContext::SetRenderTarget(nullptr);
		D3DContext::Clear();

		shader2.Bind();
		shader2.SetTexture(0, (const Texture*)&framebuffer);

		v2.Bind();
		i2.Bind();

		D3DContext::GetDeviceContext()->DrawIndexed(i2.GetCount(), 0, 0);

		w.SwapBuffers();
	}
	*/

	Game g;
	g.start();

	return 0;
}