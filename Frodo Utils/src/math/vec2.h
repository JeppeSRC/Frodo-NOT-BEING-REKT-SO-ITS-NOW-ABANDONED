#pragma once
#include "mathcommon.h"

namespace FD {

class FDAPI vec2 {
public:
	float32 x;
	float32 y;

public:
	vec2();
	//vec2(__m128 xmm);
	vec2(float32 x, float32 y);

	vec2& Add(const vec2& v);
	vec2& Add(float32 v);
	vec2& Subtract(const vec2& v);
	vec2& Subtract(float32 v);
	vec2& Multiply(const vec2& v);
	vec2& Multiply(float32 v);
	vec2& Divide(const vec2& v);
	vec2& Divide(float32 v);

	__forceinline friend vec2 operator+(const vec2& l, const vec2& r) { return vec2(l).Add(r); }
	__forceinline friend vec2 operator-(const vec2& l, const vec2& r) { return vec2(l).Subtract(r); }
	__forceinline friend vec2 operator*(const vec2& l, const vec2& r) { return vec2(l).Multiply(r); }
	__forceinline friend vec2 operator/(const vec2& l, const vec2& r) { return vec2(l).Divide(r); }

	__forceinline friend vec2 operator+(const vec2& l, float32 r) { return vec2(l).Add(r); }
	__forceinline friend vec2 operator-(const vec2& l, float32 r) { return vec2(l).Subtract(r); }
	__forceinline friend vec2 operator*(const vec2& l, float32 r) { return vec2(l).Multiply(r); }
	__forceinline friend vec2 operator/(const vec2& l, float32 r) { return vec2(l).Divide(r); }

	__forceinline void operator+=(const vec2& v) { Add(v); }
	__forceinline void operator-=(const vec2& v) { Subtract(v); }
	__forceinline void operator*=(const vec2& v) { Multiply(v); }
	__forceinline void operator/=(const vec2& v) { Divide(v); }

	__forceinline void operator+=(float32 v) { Add(v); }
	__forceinline void operator-=(float32 v) { Subtract(v); }
	__forceinline void operator*=(float32 v) { Multiply(v); }
	__forceinline void operator/=(float32 v) { Divide(v); }

	bool operator==(const vec2& v) const;
	bool operator!=(const vec2& v) const;

	__forceinline vec2 operator-() { return vec2(-x, -y); }
};



class FDAPI ivec2 {
public:
	int32 x;
	int32 y;

public:
	ivec2();
	//ivec2(__m128 xmm);
	ivec2(int32 x, int32 y);

	ivec2& Add(const ivec2& v);
	ivec2& Add(int32 v);
	ivec2& Subtract(const ivec2& v);
	ivec2& Subtract(int32 v);
	ivec2& Multiply(const ivec2& v);
	ivec2& Multiply(int32 v);
	ivec2& Divide(const ivec2& v);
	ivec2& Divide(int32 v);

	__forceinline friend ivec2 operator+(const ivec2& l, const ivec2& r) { return ivec2(l).Add(r); }
	__forceinline friend ivec2 operator-(const ivec2& l, const ivec2& r) { return ivec2(l).Subtract(r); }
	__forceinline friend ivec2 operator*(const ivec2& l, const ivec2& r) { return ivec2(l).Multiply(r); }
	__forceinline friend ivec2 operator/(const ivec2& l, const ivec2& r) { return ivec2(l).Divide(r); }

	__forceinline friend ivec2 operator+(const ivec2& l, int32 r) { return ivec2(l).Add(r); }
	__forceinline friend ivec2 operator-(const ivec2& l, int32 r) { return ivec2(l).Subtract(r); }
	__forceinline friend ivec2 operator*(const ivec2& l, int32 r) { return ivec2(l).Multiply(r); }
	__forceinline friend ivec2 operator/(const ivec2& l, int32 r) { return ivec2(l).Divide(r); }

	__forceinline void operator+=(const ivec2& v) { Add(v); }
	__forceinline void operator-=(const ivec2& v) { Subtract(v); }
	__forceinline void operator*=(const ivec2& v) { Multiply(v); }
	__forceinline void operator/=(const ivec2& v) { Divide(v); }

	__forceinline void operator+=(int32 v) { Add(v); }
	__forceinline void operator-=(int32 v) { Subtract(v); }
	__forceinline void operator*=(int32 v) { Multiply(v); }
	__forceinline void operator/=(int32 v) { Divide(v); }

	bool operator==(const ivec2& v) const;
	bool operator!=(const ivec2& v) const;

	__forceinline ivec2 operator-() { return ivec2(-x, -y); }
};

}