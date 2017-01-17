#pragma once

#include <fd.h>
#include <math/math.h>

enum FD_LIGHT_TYPE {
	FD_LIGHT_TYPE_NONE,
	FD_LIGHT_TYPE_DIRECTIONAL,
	FD_LIGHT_TYPE_POINT,
	FD_LIGHT_TYPE_SPOT,
};


class FDAPI Light {
protected:

	vec3 color;
	FD_LIGHT_TYPE lightType;

protected:
	Light(const vec3& color) { this->color = color; lightType = FD_LIGHT_TYPE_NONE; }

public:

	inline vec3& GetColor() { return color; }

	inline uint32 GetLightType() const {return lightType; }
};

class FDAPI DirectionalLight : public Light {
private:

	vec3 direction;
	float32 pad0;

public:

	DirectionalLight(const vec3& color, const vec3& direction) : Light(color), direction(direction) { lightType = FD_LIGHT_TYPE_DIRECTIONAL; }

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

	inline vec3& GetDirection() { return direction; }
	inline vec2& GetCutoffExponent() { return cutoffExponent; }
	inline float32& GetCutoff() { return cutoffExponent.x; }
	inline float32& GetExponent() { return cutoffExponent.y; }

};

class FDAPI LightStack {
private:
	List<Light*> lights;

public:
	LightStack() {}
	~LightStack() { lights.Free(); }

	void Add(Light* light) { lights.Push_back(light); }
	void Remove(Light* light) { lights.Remove(light);  }

	inline List<Light*> GetLights() const { return lights; }
	
};