#pragma once

#include <fd.h>
#include <graphics/render/model/model.h>
#include <graphics/render/material/material.h>

class FDAPI Entity {
protected:
	Model* model;
	Material* material;

	vec3 position;
	vec3 rotation;

public:
	Entity() { model = nullptr; material = nullptr; }
	Entity(const vec3& position, const vec3& rotation) : position(position), rotation(rotation) { model = nullptr; material = nullptr; }
	~Entity() { delete model, material; }


	inline Model* GetModel() const { return model; }
	inline Material* GetMaterial() const { return material; }

	inline void SetModel(Model* model) { this->model = model; }
	inline void SetMaterial(Material* material) { this->material = material; }

	inline vec3& GetPosition() { return position; }
	inline vec3& GetRotation() { return rotation; }
};