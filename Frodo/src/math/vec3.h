#pragma once
#include "mathcommon.h"

class FDAPI vec3 {
private:
	friend class vec2;
public:
	float x;
	float y;
	float z;


public:
	vec3();
	vec3(const vec2& v, float z);
	vec3(float x, float y, float z);

	vec3& Add(const vec3& v);
	vec3& Add(float v);
	vec3& Subtract(const vec3& v);
	vec3& Subtract(float v);
	vec3& Multiply(const vec3& v);
	vec3& Multiply(float v);
	vec3& Divide(const vec3& v);
	vec3& Divide(float v);

	float Dot(const vec3& v);
	vec3 Cross(const vec3& v);

	vec3& RotateX(float angle);
	vec3& RotateY(float angle);
	vec3& RotateZ(float angle);

	__forceinline friend vec3 operator+(const vec3& l, const vec3& r) { return vec3(l).Add(r); }
	__forceinline friend vec3 operator-(const vec3& l, const vec3& r) { return vec3(l).Subtract(r); }
	__forceinline friend vec3 operator*(const vec3& l, const vec3& r) { return vec3(l).Multiply(r); }
	__forceinline friend vec3 operator/(const vec3& l, const vec3& r) { return vec3(l).Divide(r); }

	__forceinline friend vec3 operator+(const vec3& l, float r) { return vec3(l).Add(r); }
	__forceinline friend vec3 operator-(const vec3& l, float r) { return vec3(l).Subtract(r); }
	__forceinline friend vec3 operator*(const vec3& l, float r) { return vec3(l).Multiply(r); }
	__forceinline friend vec3 operator/(const vec3& l, float r) { return vec3(l).Divide(r); }

	__forceinline void operator+=(const vec3& v) { Add(v); }
	__forceinline void operator-=(const vec3& v) { Subtract(v); }
	__forceinline void operator*=(const vec3& v) { Multiply(v); }
	__forceinline void operator/=(const vec3& v) { Divide(v); }

	__forceinline void operator+=(float v) { Add(v); }
	__forceinline void operator-=(float v) { Subtract(v); }
	__forceinline void operator*=(float v) { Multiply(v); }
	__forceinline void operator/=(float v) { Divide(v); }

	__forceinline vec3 operator-() { return vec3(-x, -y, -z); }
};
