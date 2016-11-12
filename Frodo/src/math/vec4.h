#pragma once
#include "mathcommon.h"

class FDAPI vec4 {
private:
	friend class vec3;
	friend class vec2;
public:
	float x;
	float y;
	float z;
	float w;
	
public:
	vec4();
	vec4(const vec2& v, float z = 0, float w = 0);
	vec4(const vec3& v, float w = 0);
	vec4(float x, float y, float z, float w);

	vec4& Add(const vec4& v);
	vec4& Add(float v);
	vec4& Subtract(const vec4& v);
	vec4& Subtract(float v);
	vec4& Multiply(const vec4& v);
	vec4& Multiply(float v);
	vec4& Divide(const vec4& v);
	vec4& Divide(float v);

	__forceinline friend vec4 operator+(const vec4& l, const vec4& r) { return vec4(l).Add(r); }
	__forceinline friend vec4 operator-(const vec4& l, const vec4& r) { return vec4(l).Subtract(r); }
	__forceinline friend vec4 operator*(const vec4& l, const vec4& r) { return vec4(l).Multiply(r); }
	__forceinline friend vec4 operator/(const vec4& l, const vec4& r) { return vec4(l).Divide(r); }

	__forceinline friend vec4 operator+(const vec4& l, float r) { return vec4(l).Add(r); }
	__forceinline friend vec4 operator-(const vec4& l, float r) { return vec4(l).Subtract(r); }
	__forceinline friend vec4 operator*(const vec4& l, float r) { return vec4(l).Multiply(r); }
	__forceinline friend vec4 operator/(const vec4& l, float r) { return vec4(l).Divide(r); }

	__forceinline void operator+=(const vec4& v) { Add(v); }
	__forceinline void operator-=(const vec4& v) { Subtract(v); }
	__forceinline void operator*=(const vec4& v) { Multiply(v); }
	__forceinline void operator/=(const vec4& v) { Divide(v); }

	__forceinline void operator+=(float v) { Add(v); }
	__forceinline void operator-=(float v) { Subtract(v); }
	__forceinline void operator*=(float v) { Multiply(v); }
	__forceinline void operator/=(float v) { Divide(v); }

	__forceinline vec4 operator-() { return vec4(-x, -y, -z, -w); }
};


class FDAPI ivec4 {
private:
	friend class ivec3;
	friend class ivec2;
public:
	int x;
	int y;
	int z;
	int w;

public:
	ivec4();
	ivec4(const ivec2& v, int z = 0, int w = 0);
	ivec4(const ivec3& v, int w = 0);
	ivec4(int x, int y, int z, int w);

	ivec4& Add(const ivec4& v);
	ivec4& Add(int v);
	ivec4& Subtract(const ivec4& v);
	ivec4& Subtract(int v);
	ivec4& Multiply(const ivec4& v);
	ivec4& Multiply(int v);
	ivec4& Divide(const ivec4& v);
	ivec4& Divide(int v);

	__forceinline friend ivec4 operator+(const ivec4& l, const ivec4& r) { return ivec4(l).Add(r); }
	__forceinline friend ivec4 operator-(const ivec4& l, const ivec4& r) { return ivec4(l).Subtract(r); }
	__forceinline friend ivec4 operator*(const ivec4& l, const ivec4& r) { return ivec4(l).Multiply(r); }
	__forceinline friend ivec4 operator/(const ivec4& l, const ivec4& r) { return ivec4(l).Divide(r); }

	__forceinline friend ivec4 operator+(const ivec4& l, int r) { return ivec4(l).Add(r); }
	__forceinline friend ivec4 operator-(const ivec4& l, int r) { return ivec4(l).Subtract(r); }
	__forceinline friend ivec4 operator*(const ivec4& l, int r) { return ivec4(l).Multiply(r); }
	__forceinline friend ivec4 operator/(const ivec4& l, int r) { return ivec4(l).Divide(r); }

	__forceinline void operator+=(const ivec4& v) { Add(v); }
	__forceinline void operator-=(const ivec4& v) { Subtract(v); }
	__forceinline void operator*=(const ivec4& v) { Multiply(v); }
	__forceinline void operator/=(const ivec4& v) { Divide(v); }

	__forceinline void operator+=(int v) { Add(v); }
	__forceinline void operator-=(int v) { Subtract(v); }
	__forceinline void operator*=(int v) { Multiply(v); }
	__forceinline void operator/=(int v) { Divide(v); }

	__forceinline ivec4 operator-() { return ivec4(-x, -y, -z, -w); }
};