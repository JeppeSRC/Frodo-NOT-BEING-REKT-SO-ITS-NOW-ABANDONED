#pragma once
#include "mathcommon.h"

class FDAPI vec2 {
public:
	float x;
	float y;

public:
	vec2();
	//vec2(__m128 xmm);
	vec2(float x, float y);

	vec2& Add(const vec2& v);
	vec2& Add(float v);
	vec2& Subtract(const vec2& v);
	vec2& Subtract(float v);
	vec2& Multiply(const vec2& v);
	vec2& Multiply(float v);
	vec2& Divide(const vec2& v);
	vec2& Divide(float v);

	__forceinline friend vec2 operator+(const vec2& l, const vec2& r) { return vec2(l).Add(r); }
	__forceinline friend vec2 operator-(const vec2& l, const vec2& r) { return vec2(l).Subtract(r); }
	__forceinline friend vec2 operator*(const vec2& l, const vec2& r) { return vec2(l).Multiply(r); }
	__forceinline friend vec2 operator/(const vec2& l, const vec2& r) { return vec2(l).Divide(r); }

	__forceinline friend vec2 operator+(const vec2& l, float r) { return vec2(l).Add(r); }
	__forceinline friend vec2 operator-(const vec2& l, float r) { return vec2(l).Subtract(r); }
	__forceinline friend vec2 operator*(const vec2& l, float r) { return vec2(l).Multiply(r); }
	__forceinline friend vec2 operator/(const vec2& l, float r) { return vec2(l).Divide(r); }

	__forceinline void operator+=(const vec2& v) { Add(v); }
	__forceinline void operator-=(const vec2& v) { Subtract(v); }
	__forceinline void operator*=(const vec2& v) { Multiply(v); }
	__forceinline void operator/=(const vec2& v) { Divide(v); }

	__forceinline void operator+=(float v) { Add(v); }
	__forceinline void operator-=(float v) { Subtract(v); }
	__forceinline void operator*=(float v) { Multiply(v); }
	__forceinline void operator/=(float v) { Divide(v); }

	__forceinline vec2 operator-() { return vec2(-x, -y); }
};



class FDAPI ivec2 {
public:
	int x;
	int y;

public:
	ivec2();
	//ivec2(__m128 xmm);
	ivec2(int x, int y);

	ivec2& Add(const ivec2& v);
	ivec2& Add(int v);
	ivec2& Subtract(const ivec2& v);
	ivec2& Subtract(int v);
	ivec2& Multiply(const ivec2& v);
	ivec2& Multiply(int v);
	ivec2& Divide(const ivec2& v);
	ivec2& Divide(int v);

	__forceinline friend ivec2 operator+(const ivec2& l, const ivec2& r) { return ivec2(l).Add(r); }
	__forceinline friend ivec2 operator-(const ivec2& l, const ivec2& r) { return ivec2(l).Subtract(r); }
	__forceinline friend ivec2 operator*(const ivec2& l, const ivec2& r) { return ivec2(l).Multiply(r); }
	__forceinline friend ivec2 operator/(const ivec2& l, const ivec2& r) { return ivec2(l).Divide(r); }

	__forceinline friend ivec2 operator+(const ivec2& l, int r) { return ivec2(l).Add(r); }
	__forceinline friend ivec2 operator-(const ivec2& l, int r) { return ivec2(l).Subtract(r); }
	__forceinline friend ivec2 operator*(const ivec2& l, int r) { return ivec2(l).Multiply(r); }
	__forceinline friend ivec2 operator/(const ivec2& l, int r) { return ivec2(l).Divide(r); }

	__forceinline void operator+=(const ivec2& v) { Add(v); }
	__forceinline void operator-=(const ivec2& v) { Subtract(v); }
	__forceinline void operator*=(const ivec2& v) { Multiply(v); }
	__forceinline void operator/=(const ivec2& v) { Divide(v); }

	__forceinline void operator+=(int v) { Add(v); }
	__forceinline void operator-=(int v) { Subtract(v); }
	__forceinline void operator*=(int v) { Multiply(v); }
	__forceinline void operator/=(int v) { Divide(v); }

	__forceinline ivec2 operator-() { return ivec2(-x, -y); }
};
