#include "deferredtest.h"

#include <graphics/font/font.h>

#define WIDTH 10
#define HWIDTH (WIDTH >> 1)
#define DEPTH 10
#define HDEPTH (DEPTH >> 1)
#define HEIGHT 2
#define HHEIGHT (HEIGHT >> 1)

void DeferredTest::OnInit() {
	window->SetVSync(1);
	SetUPS(60);

	for (unsigned int i = 0x21; i <= 0x7E; i++)
		FD_INFO("0x%02x: %c", i, i);

	FD_INFO("%d", (int)(sqrtf(0x7E - 0x21) + 1));
	
	VFS::Get()->Mount("textures", "./res/");
	VFS::Get()->Mount("models", "./res/");
	VFS::Get()->Mount("fonts", "./res/");

//	Font font("/models/arial.ttf", 32, Window::GetMonitorDpi());

	TextureManager::Add("mountains", new Texture2D("/textures/mountains.jpg"));
	TextureManager::Add("white", new Texture2D("/textures/white.png"));

	camera = new UserCamera(vec3(1, 0, -1.25), vec3(0, 0, 0));

	camera->SetProjection(mat4::Perspective(70.0f, window->GetAspectRatio(), 0.002f, 1000.0f));
	Model* model = MeshFactory::CreatePlane(WIDTH, DEPTH);
	Model* model2 = MeshFactory::CreatePlane(WIDTH, HEIGHT);
	Model* sphereModel = MeshFactory::LoadFromFile("/models/sphere.obj");
	Model* sphereModelBig = MeshFactory::LoadFromFile("/models/sphere_big.obj");
	Material* material = new Material(vec4(1, 1, 1, 1));

	bigSphere = new Entity(vec3(3.5, 0, 0), vec3(0, 0, 0), vec3(0.35, 0.35, 0.35));
	bigSphere->SetMaterial(material);
	bigSphere->SetModel(sphereModelBig);

	Entity* sphere = new Entity(vec3(0, 0, 0), vec3(0, 0, 0));
	sphere->SetModel(sphereModel);
	sphere->SetMaterial(material);
	sphere->GetScale() *= 0.55;


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

	mainRenderer = new DeferredRenderer(window);
	//mainRenderer = new ForwardRenderer(window);

	font = new Font("/fonts/verdana.ttf", 32, Window::GetMonitorDpi());

	Font::SetDefaultFont(font);

	fontRenderer = new FontRenderer(window, 500000);
	menuRenderer = new MenuRenderer(window, 1024);

	handler = new UIHandler();

	UIItem* button = new UIButton("Some text!", vec2(100, 100), vec2(500, 100));

	handler->Add(button);

	mainRenderer->SetCamera(camera);

	mainRenderer->Add(floor);
	//renderer->Add(e2);
	mainRenderer->Add(left);
	mainRenderer->Add(right);
	mainRenderer->Add(top);
	mainRenderer->Add(front);
	mainRenderer->Add(back);
	mainRenderer->Add(sphere);
	mainRenderer->Add(bigSphere);

	mainRenderer->Add(new PointLight(vec3(4, 0.0f, 0), vec3(1, 0, 1), vec3(0, 0.025, 10)));
	mainRenderer->Add(new PointLight(vec3(0, 0.0f, 4), vec3(1, 1, 0), vec3(0, 0.025, 10)));
	mainRenderer->Add(new PointLight(vec3(-4, 0.0f, 0), vec3(0, 1, 1), vec3(0, 0.025, 10)));
	mainRenderer->Add(new PointLight(vec3(0, 0.0f, -4), vec3(0, 0, 1), vec3(0, 0.025, 10)));


	mainRenderer->Add(new PointLight(vec3(4, 0.0f, 4), vec3(1, 0, 0), vec3(0, 0.025, 10)));
	mainRenderer->Add(new PointLight(vec3(-4, 0.0f, 4), vec3(0, 0, 1), vec3(0, 0.025, 10)));
	mainRenderer->Add(new PointLight(vec3(4, 0.0f, -4), vec3(1, 1, 0), vec3(0, 0.025, 10)));
	mainRenderer->Add(new PointLight(vec3(-4, 0.0f, -4), vec3(0, 1, 1), vec3(0, 0.025, 10)));

	
	spinningLight0 = new PointLight(vec3(1.5, 0.0f, 0), vec3(1, 1, 1), vec3(0, 0.025, 10));
	spinningLight1 = new PointLight(vec3(-1.5, 0.0f, 0), vec3(0, 1, 1), vec3(0, 0.025, 10));
	spinningLight2 = new PointLight(vec3(0, 0.0f, 1.5), vec3(0, 0, 1), vec3(0, 0.025, 10));
	spinningLight3 = new PointLight(vec3(0, 0.0f, -1.5), vec3(1, 0, 0), vec3(0, 0.025, 10));

	spotLight0 = new SpotLight(vec3(2, 0, 0), vec3(1, 1, 1), vec3(-1, -1, 0), vec3(0, 0, 1), vec2(25, 3));
	mainRenderer->Add(spotLight0);
	
	
	mainRenderer->Add(spinningLight0);
	mainRenderer->Add(spinningLight1);
	mainRenderer->Add(spinningLight2);
	mainRenderer->Add(spinningLight3);
	
	
}
float a = 0;
void DeferredTest::OnUpdate(float delta) {
	camera->Update(delta);
	a += 0.20 * delta;
	spotLight0->GetDirection() = camera->GetForward();
	//spotLight0->GetPosition() = camera->GetPosition();

	spinningLight0->GetPosition().RotateY(80 * delta);
	spinningLight1->GetPosition().RotateY(80 * delta);
	spinningLight2->GetPosition().RotateY(80 * delta);
	spinningLight3->GetPosition().RotateY(80 * delta);

	bigSphere->GetPosition().RotateY(-5 * delta);

	UIItem* item = handler->operator[](0);

	item->SetSize(vec2(abs(sinf(a) * 500), abs(cosf(a) * 200)));
}

void DeferredTest::OnTick() {
	FD_INFO("FPS: %u", fps);
	char fpss[32];
	sprintf(fpss, "FPS: %u", fps);
	fpsString = fpss;

	fpsString;

	fps = 0;
}

void DeferredTest::OnRender() {
	mainRenderer->Render();
	fontRenderer->Begin();
	menuRenderer->Begin();

	fontRenderer->SubmitText(fpsString, font, vec2(0, 0));
	menuRenderer->Submit(handler);

	fontRenderer->End();
	menuRenderer->End();

	fontRenderer->Render();
	menuRenderer->Render();

	fps++;
}

void DeferredTest::OnExit() {
	delete fontRenderer;
	delete menuRenderer;
}