#pragma once
#include "mathcommon.h"

class FDAPI vec2 {
private:
	// float fdata[4];
	union  {
		float fdata[4];
		struct {
			float _x;
			float _y;
			float _z;
			float _w;
		};
	};

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

	__forceinline vec2 operator-() { return vec2(-_x, -_y); }

	__forceinline float GetX() const { return _x; }
	__forceinline float GetY() const { return _y; }
};
