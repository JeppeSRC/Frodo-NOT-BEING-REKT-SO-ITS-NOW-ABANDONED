#pragma once

#include <fd.h>
#include <math/math.h>
#include <util/list.h>

namespace FD {

enum FD_LIGHT_TYPE {
	FD_LIGHT_TYPE_NONE,
	FD_LIGHT_TYPE_DIRECTIONAL = 0x1,
	FD_LIGHT_TYPE_POINT = 0x2,
	FD_LIGHT_TYPE_SPOT = 0x4,

	FD_LIGHT_CAST_SHADOW = 0x8,
};


class FDAPI Light {
protected:

	vec3 color;
	uint32 lightType;

protected:
	Light(const vec3& color) { this->color = color; lightType = FD_LIGHT_TYPE_NONE; }

public:
	inline void SetColor(vec3 color) { this->color = color; }

	inline vec3& GetColor() { return color; }

	inline uint32 GetLightType() const { return lightType; }
};

class FDAPI DirectionalLight : public Light {
private:

	vec3 direction;
	float32 pad0;

public:

	DirectionalLight(const vec3& color, const vec3& direction, bool castShadow) : Light(color), direction(direction) { lightType = FD_LIGHT_TYPE_DIRECTIONAL | (castShadow ? FD_LIGHT_CAST_SHADOW : 0); }

	inline void SetDirection(vec3 direction) { this->direction = direction; }

	inline vec3& GetDirection() { return direction; }
};

class FDAPI PointLight : public Light {
protected:

	vec3 position;
	float32 pad0;
	vec3 attenuation; // constant, linear, exponent
	float32 pad1;

public:

	PointLight(const vec3& position, const vec3& color, const vec3& attenuation) : Light(color), position(position), attenuation(attenuation) { lightType = FD_LIGHT_TYPE_POINT; }

	inline void SetAttenuation(vec3 attenuation) { this->attenuation = attenuation; }
	inline void SetPosition(vec3 position) { this->position = position; }

	inline vec3& GetPosition() { return position; }
	inline vec3& GetAttenuation() { return attenuation; }
};

class FDAPI SpotLight : public PointLight {
protected:
	vec3 direction;
	float32 pad3;
	vec2 cutoffExponent;
	vec2 pad4;

public:
	SpotLight(const vec3& position, const vec3& color, const vec3& direction, const vec3& attenuation, vec2 cutoffExponent) : PointLight(position, color, attenuation), direction(direction) {
		this->cutoffExponent.x = (float32)cosf(FD_TO_RADIANS_F(cutoffExponent.x));
		this->cutoffExponent.y = cutoffExponent.y;
		lightType = FD_LIGHT_TYPE_SPOT;
	}

	inline void SetCutoffExponent(vec2 cutoffExponent) { this->cutoffExponent = cutoffExponent; }
	inline void SetCutoff(float cutoff) { this->cutoffExponent.x = (float32)cosf(FD_TO_RADIANS_F(cutoff)); }
	inline void SetExponent(float exponent) { this->cutoffExponent.y = exponent; }

	inline vec2& GetCutoffExponent() { return cutoffExponent; }
	inline float32& GetCutoff() { return cutoffExponent.x; }
	inline float32& GetExponent() { return cutoffExponent.y; }

};

}
