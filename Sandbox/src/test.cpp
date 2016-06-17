#include "test.h"


void Test::OnInit() {
	window->SetVSync(0);

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
	a[2].position = (mat4::Translate(vec3(0, 0, -2)) * mat4::Rotate(vec3(0, 0, 45))) * vec4(-1, -1, 0, 1);*/

	v = new VertexBuffer(&a, sizeof(a), sizeof(Vertex));
	i = new IndexBuffer(indices, 3);

	v2 = new VertexBuffer(&b, sizeof(b), sizeof(Vertex));
	i2 = new IndexBuffer(indices2, 6);

	tex = new Texture2D("./res/mountains.jpg");
	framebuffer = new Framebuffer2D(window->GetWidth(), window->GetHeight(), FD_TEXTURE_FORMAT_FLOAT_32_32_32_32);

	shader = new Shader("res/vertex.hlsl", "res/pixel.hlsl");
	shader2 = new Shader("res/displayVertex.hlsl", "res/pixel.hlsl");
	
	BufferLayout layout;

	layout.Push<vec4>("POSITION");
	layout.Push<vec2>("TEXCOORDS");

	layout.CreateInputLayout(shader);
	layout.CreateInputLayout(shader2);

	D3DContext::SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mod.projection = mat4::Perspective(70.0f, window->GetAspectRatio(), 0.001f, 100.0f);
	mod.projection = mat4::Orthographic(-1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f);
	mod.model = mat4::Identity();
	//mod.model = mat4::Translate(vec3(0, 0, -2)) * mat4::Rotate(vec3(0, 0, 0));
}

void Test::OnUpdate(float delta) {
	tmp += 0.1f * delta;
	mod.model = mat4::Translate(vec3(0, 0, -0.2)) * mat4::Rotate(vec3(0, 0, tmp));
}

void Test::OnTick() {
	FD_INFO("FPS: %u", fps);
	fps = 0;
}

void Test::OnRender() {
	framebuffer->BindAsRenderTarget();
	D3DContext::Clear();

	shader->Bind();
	shader->SetVSConstantBuffer(0, &mod);
	shader->SetTexture(0, tex);


	v->Bind();
	i->Bind();

	D3DContext::GetDeviceContext()->DrawIndexed(i->GetCount(), 0, 0);
	D3DContext::SetRenderTarget(nullptr);
	D3DContext::Clear();

	shader2->Bind();
	shader2->SetTexture(0, (const Texture*)framebuffer);

	v2->Bind();
	i2->Bind();

	D3DContext::GetDeviceContext()->DrawIndexed(i2->GetCount(), 0, 0);

	fps++;
}

void Test::OnExit() {
	delete i, i2;
	delete v, v2;
	delete shader, shader2;
	delete tex;
	delete framebuffer;
}