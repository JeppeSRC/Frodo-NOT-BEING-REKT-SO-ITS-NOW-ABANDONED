#pragma once
#include "mathcommon.h"

class FDAPI vec3 {
private:
	friend class vec2;
private:
	__declspec(align(16)) union {
		__m128 _xmm;
		struct {
			float _x;
			float _y;
			float _z;
			float _w;
		};
	};



public:
	vec3();
	vec3(__m128 xmm);
	vec3(const vec2& v);
	vec3(float x, float y, float z);

	vec3& Add(const vec3& v);
	vec3& Add(float v);
	vec3& Subtract(const vec3& v);
	vec3& Subtract(float v);
	vec3& Multiply(const vec3& v);
	vec3& Multiply(float v);
	vec3& Divide(const vec3& v);
	vec3& Divide(float v);

	__forceinline friend vec3 operator+(const vec3& l, const vec3& r) { return vec3(l._xmm).Add(r); }
	__forceinline friend vec3 operator-(const vec3& l, const vec3& r) { return vec3(l._xmm).Subtract(r); }
	__forceinline friend vec3 operator*(const vec3& l, const vec3& r) { return vec3(l._xmm).Multiply(r); }
	__forceinline friend vec3 operator/(const vec3& l, const vec3& r) { return vec3(l._xmm).Divide(r); }

	__forceinline friend vec3 operator+(const vec3& l, float r) { return vec3(l._xmm).Add(r); }
	__forceinline friend vec3 operator-(const vec3& l, float r) { return vec3(l._xmm).Subtract(r); }
	__forceinline friend vec3 operator*(const vec3& l, float r) { return vec3(l._xmm).Multiply(r); }
	__forceinline friend vec3 operator/(const vec3& l, float r) { return vec3(l._xmm).Divide(r); }

	__forceinline void operator+=(const vec3& v) { Add(v); }
	__forceinline void operator-=(const vec3& v) { Subtract(v); }
	__forceinline void operator*=(const vec3& v) { Multiply(v); }
	__forceinline void operator/=(const vec3& v) { Divide(v); }

	__forceinline void operator+=(float v) { Add(v); }
	__forceinline void operator-=(float v) { Subtract(v); }
	__forceinline void operator*=(float v) { Multiply(v); }
	__forceinline void operator/=(float v) { Divide(v); }

	__forceinline float GetX() const { return _x; }
	__forceinline float GetY() const { return _y; }
	__forceinline float GetZ() const { return _z; }
};
