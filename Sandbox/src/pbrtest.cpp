#include "pbrtest.h"

using namespace FD;

void PBRTest::OnInit() {
	window->SetVSync(0);

	camera = new UserCamera(vec3(0, 0, 0), vec3(0, 0, 0));
	camera->SetProjection(mat4::Perspective(70.0f, window->GetAspectRatio(), 0.001f, 1000.0f));

	VFS::Get()->Mount("shaders", "res/pbr/shaders/");
	VFS::Get()->Mount("models", "res/");

	scene = new Scene(window);

	scene->SetCamera(camera);

	Shader* shader = new Shader("/shaders/pbr_lighting_v.hlsl", "/shaders/pbr_lighting_p.hlsl");
	shader->ShaderGenComplete();

	BufferLayout l;
	l.Push<vec3>("POSITION");
	l.Push<vec3>("NORMAL");
	l.Push<vec2>("TEXCOORD");

	l.CreateInputLayout(shader);

	Font::FD_RANGE<> r;

	r.start = 0x20;
	r.end = 0x7E;

	Font* s = new Font("res/arial.ttf", 50, Window::GetMonitorDpi(), &r, 1);

	Shader* skyboxShader = new Shader("/shaders/pbr_skybox_v.hlsl", "/shaders/pbr_skybox_p.hlsl");
	skyboxShader->ShaderGenComplete();

	l.CreateInputLayout(skyboxShader);

	TextureCube* cubeMap = new TextureCube(new String[2]{ "res/cubemap.png", "" });

	Material* skybox = new Material(skyboxShader);
	skybox->SetTexture("m_EnvironmentMap", cubeMap);
	skybox->SetVCBuffer("InverseViewMatrix", (void*)mat4::Inverse(camera->GetViewMatrix()).GetData());

	skyboxMaterial = new MaterialInstance(skybox);

	Texture* albedo = new Texture2D("res/bricks.jpg");
	Texture* metallic = new Texture2D("res/metallic.png");
	Texture* roughness = new Texture2D("res/roughness.png");
	Texture* ao = new Texture2D("res/ao.png");
	
	struct MATERIAL {
		vec3 albedo = vec3(0.5f , 0, 0);
		float albedoFactor = 0;
		float metallic = 1.0f;
		float metallicFactor = 0;
		float roughness = 0.521;
		float roughnessFactor = 0.25;
		float ambientOcclusion = 1;
		float ambientOcclusionFactor = 0;
		vec2 pad0;
	} materialData;

	Material* material = new Material(shader);
	material->SetPCBuffer("Material", (void*)&materialData);
	
	material->SetTexture("m_AlbedoMap", albedo);
	material->SetTexture("m_EnvironmentMap", cubeMap);

	MaterialInstance* mat = new MaterialInstance(material);

	Mesh* sphere = MeshFactory::LoadFromFile("/models/sphere.obj", mat);

	Entity3D* e = new Entity3D(vec3(0, 5, 0), vec3(0, 0, 0));
	e->SetMesh(sphere);

	Entity3D* e2 = new Entity3D(vec3(3, 5, 0), vec3(0, 0, 0));
	e2->SetMesh(sphere);

	Entity3D* e3 = new Entity3D(vec3(-3, 5, 0), vec3(0, 0, 0));
	e3->SetMesh(sphere);

	MaterialInstance* mi = new MaterialInstance(material);

	mi->SetPCBufferElement("m_AlbedoFactor", 1);
	mi->SetPCBufferElement("m_MetallicFactor", 1);
	mi->SetPCBufferElement("m_RoughnessFactor", 1);

	mi->SetTexture("m_AlbedoMap", albedo);
	mi->SetTexture("m_MetallicMap", metallic);
	mi->SetTexture("m_RoughnessMap", roughness);
	mi->SetTexture("m_AmbientOcclusionMap", ao);

	Entity3D* floor = new Entity3D(vec3(0, -1, 0), vec3(90, 0, 0));
	floor->SetMesh(MeshFactory::CreatePlane(vec2(10, 10), mi));

	Entity3D* sky = new Entity3D(vec3(0, 0, 0), vec3(0, 0, 0));
	sky->SetMesh(MeshFactory::CreatePlane(2, 2, skyboxMaterial));
	
//	scene->Add(sky);
	scene->Add(e);
	scene->Add(e2);
	scene->Add(e3);
	scene->Add(floor);
	
	scene->Add(new PointLight(vec3(0, -0.25f, -0), vec3(1, 1, 1), vec3(0, 0, 0)));
}

void PBRTest::OnUpdate(float delta) {
	camera->Update(delta);
	scene->Update(delta);

	skyboxMaterial->SetVCBufferElement(0, (void*)mat4::Inverse(camera->GetViewMatrix()).GetData());
}

void PBRTest::OnTick() {
	FD_INFO("FPS: %u", fps);
	fps = 0;
}

void PBRTest::OnRender() {
	scene->Render();
	fps++;
}

void PBRTest::OnExit() {

}