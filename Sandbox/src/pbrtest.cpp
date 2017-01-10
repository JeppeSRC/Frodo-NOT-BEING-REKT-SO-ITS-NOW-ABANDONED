#include "pbrtest.h"

void PBRTest::OnInit() {
	window->SetVSync(1);

	camera = new UserCamera(vec3(0, 0, 0), vec3(0, 0, 0));
	camera->SetProjection(mat4::Perspective(70.0f, window->GetAspectRatio(), 0.001f, 100.0f));

	VFS::Get()->Mount("shaders", "res/pbr/shaders/");

	Shader* environmentShader = new Shader("/shaders/pbr_environment.v", "/shaders/pbr_environment.p");
	environmentShader->ShaderGenComplete();

	BufferLayout l;
	l.Push<vec3>("POSITION");
	l.Push<vec3>("TEXCOORD");

	l.CreateInputLayout(environmentShader);

	renderer = new PBRStaticRenderer(window, nullptr, environmentShader, camera);
	renderer->SetEnvironment(new PBREnvironment(vec3(1.5f, 1.5f, 1.5f), new TextureCube(new String[6]{ "res/testmap.png", ""})));
}

void PBRTest::OnUpdate(float delta) {
	camera->Update(delta);
}

void PBRTest::OnTick() {

}

void PBRTest::OnRender() {
	renderer->Render();
}

void PBRTest::OnExit() {
	
}