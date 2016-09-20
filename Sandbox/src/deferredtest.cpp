#include "deferredtest.h"

#define WIDTH 10
#define HWIDTH (WIDTH >> 1)
#define DEPTH 10
#define HDEPTH (DEPTH >> 1)
#define HEIGHT 2
#define HHEIGHT (HEIGHT >> 1)

void DeferredTest::OnInit() {
	window->SetVSync(0);
	SetUPS(60);

	VFS::Get()->Mount("textures", "./res/");
	VFS::Get()->Mount("models", "./res/");

	TextureManager::Add("mountains", new Texture2D("/textures/mountains.jpg"));
	//TextureManager::Add("white", new Texture2D("/textures/white.png"));

	camera = new UserCamera(vec3(1, 0, -2), vec3(0, 0, 0));


	Model* model = MeshFactory::CreatePlane(WIDTH, DEPTH);
	Model* model2 = MeshFactory::CreatePlane(WIDTH, HEIGHT);
	Model* sphereModel = MeshFactory::LoadFromFile("/models/sphere.obj");
	Material* material = new Material(vec4(1, 1, 1, 1), new Texture2D("/textures/white.png"));

	Entity* sphere = new Entity(vec3(0, 0, 0), vec3(0, 0, 0));
	sphere->SetModel(sphereModel);
	sphere->SetMaterial(material);
	sphere->GetScale() *= 0.35;


	Entity* floor = new Entity({0, -0.5f, 0}, {90, 0, 0});
	floor->SetModel(model);
	floor->SetMaterial(material);


	Entity* e2 = new Entity({0, 0.5f, 0}, {35, 45, 45});
	e2->SetMaterial(material);
	e2->SetModel(MeshFactory::CreateCube(0.21, 0.21, 0.21));

	Entity* left = new Entity({-HWIDTH, -0.5f + HHEIGHT, 0}, {0, 90, 0});
	left->SetMaterial(material);
	left->SetModel(model2);

	Entity* right = new Entity({HWIDTH, -0.5f + HHEIGHT, 0}, {0, -90, 0});
	right->SetMaterial(material);
	right->SetModel(model2);

	Entity* top = new Entity({0, -0.5f + HEIGHT, 0}, {-90, 0, 0});
	top->SetMaterial(material);
	top->SetModel(model);

	Entity* front = new Entity({0, -0.5f + HHEIGHT, HDEPTH}, {0, 0, 0});
	front->SetMaterial(material);
	front->SetModel(model2);

	Entity* back = new Entity({0, -0.5f + HHEIGHT, -HDEPTH}, {0, 180, 0});
	back->SetMaterial(material);
	back->SetModel(model2);

	renderer = new DeferredRenderer(window->GetWidth(), window->GetHeight());
	renderer->SetCamera(camera);

	renderer->AddEntity(floor);
	//renderer->AddEntity(e2);
	renderer->AddEntity(left);
	renderer->AddEntity(right);
	renderer->AddEntity(top);
	renderer->AddEntity(front);
	renderer->AddEntity(back);
	renderer->AddEntity(sphere);
	
//	renderer->AddLight(new DirectionalLight(vec3(1, 0, 1), vec3(0, -1.f, 0)));

	//renderer->AddLight(new PointLight(vec3(0, 0.0f, 0), vec3(1, 1, 1), vec3(1, 0.025, 10)));


	renderer->AddLight(new PointLight(vec3(4, 0.0f, 0), vec3(1, 0, 1), vec3(1, 0.025, 4)));
	renderer->AddLight(new PointLight(vec3(0, 0.0f, 4), vec3(1, 1, 0), vec3(1, 0.025, 4)));
	renderer->AddLight(new PointLight(vec3(-4, 0.0f, 0), vec3(0, 1, 1), vec3(1, 0.025, 4)));
	renderer->AddLight(new PointLight(vec3(0, 0.0f, -4), vec3(0, 0, 1), vec3(1, 0.025, 4)));


	renderer->AddLight(new PointLight(vec3(4, 0.0f, 4), vec3(1, 0, 0), vec3(1, 0.025, 4)));
	renderer->AddLight(new PointLight(vec3(-4, 0.0f, 4), vec3(0, 0, 1), vec3(1, 0.025, 4)));
	renderer->AddLight(new PointLight(vec3(4, 0.0f, -4), vec3(1, 1, 0), vec3(1, 0.025, 4)));
	renderer->AddLight(new PointLight(vec3(-4, 0.0f, -4), vec3(0, 1, 1), vec3(1, 0.025, 4)));

	
	spinningLight0 = new PointLight(vec3(1.5, 0.0f, 0), vec3(1, 1, 1), vec3(1, 0.025, 4));
	spinningLight1 = new PointLight(vec3(-1.5, 0.0f, 0), vec3(0, 1, 1), vec3(1, 0.025, 4));
	spinningLight2 = new PointLight(vec3(0, 0.0f, 1.5), vec3(0, 0, 1), vec3(1, 0.025, 4));
	spinningLight3 = new PointLight(vec3(0, 0.0f, -1.5), vec3(1, 0, 0), vec3(1, 0.025, 4));

	spotLight0 = new SpotLight(vec3(2, 0, 0), vec3(1, 1, 1), vec3(-1, -1, 0), vec3(0, 0, 6), vec2(25, 3));

	renderer->AddLight(spotLight0);

	// 111
	// 011
	// 001
	// 100
	// 110
	// 101
	// 010
	
	renderer->AddLight(spinningLight0);
	renderer->AddLight(spinningLight1);
	renderer->AddLight(spinningLight2);
	renderer->AddLight(spinningLight3);

	

	/*renderer->AddLight(new PointLight(vec3(0, 0.0f, 0), vec3(1, 1, 1), vec3(1, 0.025, 1)));


	renderer->AddLight(new PointLight(vec3(4, 0.0f, 0), vec3(1, 1, 1), vec3(1, 0.025, 1)));
	renderer->AddLight(new PointLight(vec3(0, 0.0f, 4), vec3(1, 1, 1), vec3(1, 0.025, 1)));
	renderer->AddLight(new PointLight(vec3(-4, 0.0f, 0), vec3(1, 1, 1), vec3(1, 0.025, 1)));
	renderer->AddLight(new PointLight(vec3(0, 0.0f, -4), vec3(1, 1, 1), vec3(1, 0.025, 1)));


	renderer->AddLight(new PointLight(vec3(4, 0.0f, 4), vec3(1, 1, 1), vec3(1, 0.025, 1)));
	renderer->AddLight(new PointLight(vec3(-4, 0.0f, 4), vec3(1, 1, 1), vec3(1, 0.025, 1)));
	renderer->AddLight(new PointLight(vec3(4, 0.0f, -4), vec3(1, 1, 1), vec3(1, 0.025, 1)));
	renderer->AddLight(new PointLight(vec3(-4, 0.0f, -4), vec3(1, 1, 1), vec3(1, 0.025, 1)));*/
	
}

void DeferredTest::OnUpdate(float delta) {
	camera->Update(delta);

	spotLight0->GetDirection() = camera->GetForward();
	//spotLight0->GetPosition() = camera->GetPosition();

	spinningLight0->GetPosition().RotateY(80 * delta);
	spinningLight1->GetPosition().RotateY(80 * delta);
	spinningLight2->GetPosition().RotateY(80 * delta);
	spinningLight3->GetPosition().RotateY(80 * delta);


	FD_INFO("%f %f", spotLight0->GetCutoff(), spotLight0->GetExponent());
}

void DeferredTest::OnTick() {
	FD_INFO("FPS: %u", fps);
	fps = 0;
}

void DeferredTest::OnRender() {
	renderer->Render();

	fps++;
}

void DeferredTest::OnExit() {
	delete renderer;
}