#pragma once

#include <fd.h>
#include <graphics/texture/texture2d.h>
#include <graphics/texture/texturemanager.h>
#include <math/math.h>

class Material {
protected:

	vec4 diffuseColor;
	Texture2D* diffuseTexture;

public:

	Material(const vec4& diffuseColor, Texture2D* diffuseTexture = (Texture2D*)TextureManager::Get("default")) : diffuseColor(diffuseColor), diffuseTexture(diffuseTexture) { }


	inline vec4& GetDiffuseColor() { return diffuseColor; }
	inline Texture2D* GetDiffuseTexture() { return diffuseTexture; }
};