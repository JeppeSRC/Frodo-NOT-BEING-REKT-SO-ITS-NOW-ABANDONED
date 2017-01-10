#pragma once

#include <fd.h>
#include <graphics/pbr/material/pbrmaterial.h>
#include <graphics/render/light/light.h>
#include <graphics/shader/shader.h>
#include <graphics/render/camera/camera.h>
#include <graphics/pbr/material/pbrenvironment.h>
#include <entity/entity.h>

class FDAPI PBRRenderer {
public:
	struct FD_PBR_RENDER_CMD {
		Model* model;
		PBRMaterial* material;

		vec3 position;
		vec3 rotation;
		vec3 scale;
	};
protected:

	PBRRenderer();
public:
	virtual ~PBRRenderer();

	virtual void Add(Entity* e) = 0;
	virtual void Add(FD_PBR_RENDER_CMD cmd) = 0;
	virtual void Add(PointLight* light) = 0;

	virtual void Render() = 0;
};


class FDAPI PBRStaticRenderer : public PBRRenderer {
private:
	Window* window;
	Shader* shader;
	Shader* environmentShader;
	Camera* camera;

	PBREnvironment* environment;

	List<PointLight*> pointLights;
	List<Entity*> entities;
	List<FD_PBR_RENDER_CMD> commands;


public:
	PBRStaticRenderer(Window* window, Shader* shader, Shader* environmetShader, Camera* camera);
	~PBRStaticRenderer();

	void Render() override;
	void Add(Entity* e) override;
	void Add(FD_PBR_RENDER_CMD cmd) override;
	void Add(PointLight* light) override;

	inline void SetWindow(Window* window) { this->window = window; }
	inline void SetShader(Shader* shader) { this->shader = shader; }
	inline void SetEnvironmentShader(Shader* shader) { this->environmentShader = shader; }
	inline void SetEnvironment(PBREnvironment* environment) { this->environment = environment; }
	inline void SetCamera(Camera* camera) { this->camera = camera; }

	inline Window* GetWindow() const { return window; }
	inline Shader* GetShader() const { return shader; }
	inline Camera* GetCamera() const { return camera; }
};