#pragma once

#include <fd.h>
#include <math/math.h>


class FDAPI Light {
protected:

	vec3 color;
	float pad;

protected:
	Light(const vec3& color) { this->color = color; }

public:

	inline vec3& GetColor() { return color; }

};

class FDAPI DirectionalLight : public Light {
private:

	vec3 direction;
	float pad0;

public:

	DirectionalLight(const vec3& color, const vec3& direction) : Light(color), direction(direction) {  }

	inline vec3& GetDirection() { return direction; }
};

class FDAPI PointLight : public Light {
protected:

	vec3 position;
	float pad0;
	vec3 attenuation; // constant, linear, exponent
	float pad1;

public:

	PointLight(const vec3& position, const vec3& color, const vec3& attenuation) : Light(color), position(position), attenuation(attenuation) { }
	
	inline vec3& GetPosition() { return position; }
	inline vec3& GetAttenuation() { return attenuation; }
};

class FDAPI SpotLight : public PointLight {
protected:
	vec3 direction;
	float pad3;
	vec2 cutoffExponent;
	vec2 pad4;

public:
	SpotLight(const vec3& position, const vec3& color, const vec3& direction, const vec3& attenuation, vec2 cutoffExponent) : PointLight(position, color, attenuation), direction(direction) { 
		this->cutoffExponent.x = cosf(FD_TO_RADIANS(cutoffExponent.x));
		this->cutoffExponent.y = cutoffExponent.y;

	}

	inline vec3& GetDirection() { return direction; }
	inline vec2& GetCutoffExponent() { return cutoffExponent; }
	inline float& GetCutoff() { return cutoffExponent.x; }
	inline float& GetExponent() { return cutoffExponent.y; }

};