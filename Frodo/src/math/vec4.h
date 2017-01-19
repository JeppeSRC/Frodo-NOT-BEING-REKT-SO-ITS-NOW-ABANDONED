#pragma once
#include "mathcommon.h"

class FDAPI vec4 {
private:
	friend class vec3;
	friend class vec2;
public:
	float32 x;
	float32 y;
	float32 z;
	float32 w;
	
public:
	vec4();
	vec4(const vec2& v, float32 z = 0, float32 w = 0);
	vec4(const vec3& v, float32 w = 0);
	vec4(float32 x, float32 y, float32 z, float32 w);

	vec4& Add(const vec4& v);
	vec4& Add(float32 v);
	vec4& Subtract(const vec4& v);
	vec4& Subtract(float32 v);
	vec4& Multiply(const vec4& v);
	vec4& Multiply(float32 v);
	vec4& Divide(const vec4& v);
	vec4& Divide(float32 v);

	__forceinline friend vec4 operator+(const vec4& l, const vec4& r) { return vec4(l).Add(r); }
	__forceinline friend vec4 operator-(const vec4& l, const vec4& r) { return vec4(l).Subtract(r); }
	__forceinline friend vec4 operator*(const vec4& l, const vec4& r) { return vec4(l).Multiply(r); }
	__forceinline friend vec4 operator/(const vec4& l, const vec4& r) { return vec4(l).Divide(r); }

	__forceinline friend vec4 operator+(const vec4& l, float32 r) { return vec4(l).Add(r); }
	__forceinline friend vec4 operator-(const vec4& l, float32 r) { return vec4(l).Subtract(r); }
	__forceinline friend vec4 operator*(const vec4& l, float32 r) { return vec4(l).Multiply(r); }
	__forceinline friend vec4 operator/(const vec4& l, float32 r) { return vec4(l).Divide(r); }

	__forceinline void operator+=(const vec4& v) { Add(v); }
	__forceinline void operator-=(const vec4& v) { Subtract(v); }
	__forceinline void operator*=(const vec4& v) { Multiply(v); }
	__forceinline void operator/=(const vec4& v) { Divide(v); }

	__forceinline void operator+=(float32 v) { Add(v); }
	__forceinline void operator-=(float32 v) { Subtract(v); }
	__forceinline void operator*=(float32 v) { Multiply(v); }
	__forceinline void operator/=(float32 v) { Divide(v); }

	__forceinline vec4 operator-() { return vec4(-x, -y, -z, -w); }
};


class FDAPI ivec4 {
private:
	friend class ivec3;
	friend class ivec2;
public:
	int32 x;
	int32 y;
	int32 z;
	int32 w;

public:
	ivec4();
	ivec4(const ivec2& v, int32 z = 0, int32 w = 0);
	ivec4(const ivec3& v, int32 w = 0);
	ivec4(int32 x, int32 y, int32 z, int32 w);

	ivec4& Add(const ivec4& v);
	ivec4& Add(int32 v);
	ivec4& Subtract(const ivec4& v);
	ivec4& Subtract(int32 v);
	ivec4& Multiply(const ivec4& v);
	ivec4& Multiply(int32 v);
	ivec4& Divide(const ivec4& v);
	ivec4& Divide(int32 v);

	__forceinline friend ivec4 operator+(const ivec4& l, const ivec4& r) { return ivec4(l).Add(r); }
	__forceinline friend ivec4 operator-(const ivec4& l, const ivec4& r) { return ivec4(l).Subtract(r); }
	__forceinline friend ivec4 operator*(const ivec4& l, const ivec4& r) { return ivec4(l).Multiply(r); }
	__forceinline friend ivec4 operator/(const ivec4& l, const ivec4& r) { return ivec4(l).Divide(r); }

	__forceinline friend ivec4 operator+(const ivec4& l, int32 r) { return ivec4(l).Add(r); }
	__forceinline friend ivec4 operator-(const ivec4& l, int32 r) { return ivec4(l).Subtract(r); }
	__forceinline friend ivec4 operator*(const ivec4& l, int32 r) { return ivec4(l).Multiply(r); }
	__forceinline friend ivec4 operator/(const ivec4& l, int32 r) { return ivec4(l).Divide(r); }

	__forceinline void operator+=(const ivec4& v) { Add(v); }
	__forceinline void operator-=(const ivec4& v) { Subtract(v); }
	__forceinline void operator*=(const ivec4& v) { Multiply(v); }
	__forceinline void operator/=(const ivec4& v) { Divide(v); }

	__forceinline void operator+=(int32 v) { Add(v); }
	__forceinline void operator-=(int32 v) { Subtract(v); }
	__forceinline void operator*=(int32 v) { Multiply(v); }
	__forceinline void operator/=(int32 v) { Divide(v); }

	__forceinline ivec4 operator-() { return ivec4(-x, -y, -z, -w); }
};