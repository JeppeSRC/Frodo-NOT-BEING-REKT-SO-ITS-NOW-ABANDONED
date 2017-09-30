#include "rTest.h"

using namespace FD;

void RTest::OnCreateWindow() {
	FD_WINDOW_PROPERTIES prop;

	prop.width = 1280;
	prop.height = 720;

	window = new Window("LeL", prop, D3DFactory::GetAdapters()[0], nullptr);
	window->SetVSync(1);
}

void RTest::OnInit() {
	camera = new UserCamera(vec3(0, 0, -4), vec3(0, 0, 0));
	camera->SetProjection(mat4::Perspective(80.0f, 16.0f / 9.0f, 0.001f, 1000));
	renderer = new SimpleRenderer(window);

	Material mat(renderer->GetBaseMaterial());

	D3D11_SAMPLER_DESC d = Sampler::GetDefaultDesc();

	d.Filter = D3D11_FILTER_ANISOTROPIC;
	d.MaxAnisotropy = 16;

	Sampler* samp = new Sampler(d);
	mat.SetSampler("samp", samp);

	d.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	d.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	d.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;

	d.BorderColor[0] = 1.0f;
	d.BorderColor[1] = 1.0f;
	d.BorderColor[2] = 1.0f;
	d.BorderColor[3] = 1.0f;

	samp = new Sampler(d);
	mat.SetSampler("shadowSamp", samp);

	mat.SetPCBuffer("Material", nullptr);
	mat.SetPCBufferElement("material.color", vec3(1, 1, 1));

	Material* cubeMat = new Material(mat);
	cubeMat->SetTexture("diffuse", new Texture2D("res/bricks.jpg"));

	Material* monkeyMat = new Material(mat);
	monkeyMat->SetTexture("diffuse", new Texture2D("res/cube.png"));

	Mesh* cubeMesh = MeshFactory::CreateCube(1, 1, 1, cubeMat);
	Mesh* monkeyMesh = MeshFactory::LoadFromFile("res/monkey/monkey.obj", monkeyMat, false);

	cube = new Entity3D(vec3(0, 0, 0), vec3(0, 0, 0), vec3(1.2, 0.5, 1.2));
	cube->SetMesh(cubeMesh);
	cube->SetFlag(FD_ENTITY_CAST_SHADOW);

	Material* planeMat = new Material(mat);
	planeMat->SetTexture("diffuse", new Texture2D("res/white.png"));

	floor = new Entity3D(vec3(0, -1.5f, 0), vec3(90, 0, 0));
	floor->SetMesh(MeshFactory::CreatePlane(100, 100, planeMat));
	floor->SetFlag(FD_ENTITY_CAST_SHADOW);

	cube2 = new Entity3D(vec3(0, 0, 0), vec3(0, 0, 0), vec3(0.5f, 0.5f, 0.5f));
	cube2->SetMesh(cubeMesh);
	//cube2->SetFlag(FD_ENTITY_CAST_SHADOW);

	Entity3D* monkey = new Entity3D(vec3(3, 0, -1.6), vec3(0, 0, 0));
	monkey->SetMesh(monkeyMesh);
	monkey->SetFlag(FD_ENTITY_CAST_SHADOW);

	renderer->Submit(floor);
	renderer->Submit(cube2);
	renderer->Submit(cube);
	renderer->Submit(monkey);

//	renderer->Submit(new PointLight(vec3(0, 0, 4), vec3(1, 1, 1), vec3(0.8f, 1.8f, 0)));
//	renderer->Submit(new PointLight(vec3(0, 0, -4), vec3(1, 1, 1), vec3(0.8f, 1.8f, 0)));
//	renderer->Submit(new PointLight(vec3(0, 4, 0), vec3(1, 1, 1), vec3(0.8f, 1.8f, 0)));
//	renderer->Submit(pLight = new PointLight(vec3(1, 2, 0), vec3(1, 1, 1), vec3(0.6f, 0.4f, 0), false));
//	renderer->Submit(new PointLight(vec3(-4, 0, 0), vec3(1, 1, 1), vec3(0.8f, 1.8f, 0)));
	
	renderer->Submit(dLight = new DirectionalLight(vec3(0.3, 0.3, 0.3), vec3(0, -0.8, 1), true));
}

void RTest::OnTick() {
	FD_INFO("FPS: %u", fps);
	fps = 0;
}
float aa = 0;
void RTest::OnUpdate(float delta) {
	camera->Update(delta);

//	dLight->GetDirection().RotateY(10 * delta);
//	pLight->GetPosition().RotateY(20 * delta);
//	cube2->SetPosition(pLight->GetPosition());
}

void RTest::OnRender() {
	renderer->Begin(camera);
	renderer->Present();

	fps++;
}

bool RTest::OnKeyboardActionKeyPressed(FD_KEY key) {
	if (key == FD_KEY_R) renderer->Remove(renderer->GetLights()[0]->light);
	return false;
}