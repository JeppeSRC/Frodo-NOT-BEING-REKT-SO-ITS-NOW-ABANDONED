#pragma once
#include "mathcommon.h"

class FDAPI vec4 {
private:
	friend class vec3;
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
	vec4();
	vec4(__m128 xmm);
	vec4(const vec2& v);
	vec4(const vec3& v);
	vec4(float x, float y, float z, float w);

	vec4& Add(const vec4& v);
	vec4& Add(float v);
	vec4& Subtract(const vec4& v);
	vec4& Subtract(float v);
	vec4& Multiply(const vec4& v);
	vec4& Multiply(float v);
	vec4& Divide(const vec4& v);
	vec4& Divide(float v);

	__forceinline friend vec4 operator+(const vec4& l, const vec4& r) { return vec4(l._xmm).Add(r); }
	__forceinline friend vec4 operator-(const vec4& l, const vec4& r) { return vec4(l._xmm).Subtract(r); }
	__forceinline friend vec4 operator*(const vec4& l, const vec4& r) { return vec4(l._xmm).Multiply(r); }
	__forceinline friend vec4 operator/(const vec4& l, const vec4& r) { return vec4(l._xmm).Divide(r); }

	__forceinline friend vec4 operator+(const vec4& l, float r) { return vec4(l._xmm).Add(r); }
	__forceinline friend vec4 operator-(const vec4& l, float r) { return vec4(l._xmm).Subtract(r); }
	__forceinline friend vec4 operator*(const vec4& l, float r) { return vec4(l._xmm).Multiply(r); }
	__forceinline friend vec4 operator/(const vec4& l, float r) { return vec4(l._xmm).Divide(r); }

	__forceinline void operator+=(const vec4& v) { Add(v); }
	__forceinline void operator-=(const vec4& v) { Subtract(v); }
	__forceinline void operator*=(const vec4& v) { Multiply(v); }
	__forceinline void operator/=(const vec4& v) { Divide(v); }

	__forceinline void operator+=(float v) { Add(v); }
	__forceinline void operator-=(float v) { Subtract(v); }
	__forceinline void operator*=(float v) { Multiply(v); }
	__forceinline void operator/=(float v) { Divide(v); }

	__forceinline float x() const { return _x; }
	__forceinline float y() const { return _y; }
	__forceinline float z() const { return _z; }
	__forceinline float w() const { return _w; }
};
