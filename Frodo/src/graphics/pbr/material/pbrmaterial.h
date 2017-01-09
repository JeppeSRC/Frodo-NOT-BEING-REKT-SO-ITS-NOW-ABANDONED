#pragma once

#include <fd.h>
#include <graphics/render/material/material.h>
#include <graphics/texture/texture2d.h>
#include <math/vec3.h>

class FDAPI PBRMaterial : private Material {
protected:
	vec3  albedoValue = vec3(1.0f, 1.0f, 1.0f);
	float metallicValue = 0.0f;
	float roughnessValue = 0.0f;
	float aoValue = 1.0f;

	float useAlbedoMap = 0.0f;
	float useMetallicMap = 0.0f;
	float useRoughnessMap = 0.0f;
	float useAOMap = 0.0f;

	Texture2D* albedo = nullptr;
	Texture2D* metallic = nullptr;
	Texture2D* roughness = nullptr;
	Texture2D* ao = nullptr;
public:
	PBRMaterial() {}

	inline void SetAlbedoMap(Texture2D* albedo) { this->albedo = albedo; UseAlbedoMap(true); }
	inline void SetMetallicMap(Texture2D* metallic) { this->metallic = metallic; UseMetallicMap(true); }
	inline void SetRoughnessMap(Texture2D* roughness) { this->roughness = roughness; UseRoughnessMap(true); }
	inline void SetAOMap(Texture2D* ao) { this->ao = ao; UseAOMap(true); }

	inline void SetAlbedo(const vec3& albedoValue) { this->albedoValue = albedoValue; }
	inline void SetMetallic(float metallicValue) { this->metallicValue = metallicValue; }
	inline void SetRoughness(float roughnessValue) { this->roughnessValue = roughnessValue; }
	inline void SetAO(float aoValue) { this->aoValue = aoValue; }

	inline void UseAlbedoMap(bool use) { useAlbedoMap = use ? 1.0f : 0.0f; }
	inline void UseMetallicMap(bool use) { useMetallicMap = use ? 1.0f : 0.0f; }
	inline void UseRoughnessMap(bool use) { useRoughnessMap = use ? 1.0f : 0.0f; }
	inline void UseAOMap(bool use) { useAOMap = use ? 1.0f : 0.0f; }


	inline Texture2D* GetAlbedoMap() const { return albedo; }
	inline Texture2D* GetMetallicMap() const { return metallic; }
	inline Texture2D* GetRoughnessMap() const { return roughness; }
	inline Texture2D* GetAOMap() const { return roughness; }

	inline vec3 GetAlbedo() const { return albedoValue; }
	inline float GetMetallic() const { return metallicValue; }
	inline float GetRoughness() const { return roughnessValue; }
	inline float GetAO() const { return aoValue; }
};
