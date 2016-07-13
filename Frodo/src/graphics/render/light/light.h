#pragma once

#include <fd.h>
#include <math/vec3.h>


class FDAPI Light {
protected:

	vec3 color;

	Light(const vec3& color) { this->color = color; }

public:

	inline vec3& GetColor() { return color; }

};

class FDAPI DirectionalLight : public Light {
private:

	vec3 direction;

public:

	DirectionalLight(const vec3& color, const vec3& direction) : Light(color), direction(direction) {  }

	inline vec3& GetDirection() { return direction; }
};

class FDAPI PointLight : public Light {
private:

	vec3 position;
	vec3 attenuation; // constant, linear, exponent

public:

	PointLight(const vec3& position, const vec3& color, const vec3& attenuation) : Light(color), position(position), attenuation(attenuation) { }
	PointLight(const vec3& position, const vec3& color, float constant, float linear, float exponent) : PointLight(position, color, vec3(constant, linear, exponent)) { }

	inline vec3& GetPosition() { return position; }
	inline vec3& GetAttenuation() { return attenuation; }
};