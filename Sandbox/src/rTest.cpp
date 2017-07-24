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
	camera->SetProjection(mat4::Perspective(60.0f, 16.0f / 9.0f, 0.001f, 1000));
	renderer = new SimpleRenderer(window);

	Material mat(renderer->GetBaseMaterial());

	mat.SetPCBuffer("Material", nullptr);
	mat.SetPCBufferElement("material.color", vec3(1, 1, 1));

	Material* cubeMat = new Material(mat);
	cubeMat->SetTexture("diffuse", new Texture2D("res/bricks.jpg"));

	cube = new Entity3D(vec3(0, 0, 0), vec3(0, 0, 0));
	cube->SetMesh(MeshFactory::CreateCube(1.5f, 1.5f, 1.5f, cubeMat));

	Material* planeMat = new Material(mat);
	planeMat->SetTexture("diffuse", new Texture2D("res/white.png"));

	floor = new Entity3D(vec3(0, -1.5f, 0), vec3(90, 0, 0));
	floor->SetMesh(MeshFactory::CreatePlane(100, 100, planeMat));

	renderer->Submit(floor);
	renderer->Submit(cube);

	renderer->Submit(new PointLight(vec3(0, 0, 4), vec3(1, 1, 1), vec3(0.8f, 1.8f, 0)));
//	renderer->Submit(new PointLight(vec3(0, 0, -4), vec3(1, 1, 1), vec3(0.8f, 1.8f, 0)));
//	renderer->Submit(new PointLight(vec3(0, 4, 0), vec3(1, 1, 1), vec3(0.8f, 1.8f, 0)));
	renderer->Submit(new PointLight(vec3(4, 0, 0), vec3(1, 1, 1), vec3(0.8f, 1.8f, 0)));
//	renderer->Submit(new PointLight(vec3(-4, 0, 0), vec3(1, 1, 1), vec3(0.8f, 1.8f, 0)));

	renderer->Submit(dLight = new DirectionalLight(vec3(0.2, 0.2, 0.2), vec3(1, -0.8, 1), true));
}

void RTest::OnTick() {
	FD_INFO("FPS: %u", fps);
	fps = 0;
}

void RTest::OnUpdate(float delta) {
	camera->Update(delta);

	dLight->GetDirection().RotateY(10 * delta);
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