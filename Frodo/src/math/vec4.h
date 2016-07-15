#pragma once
#include "mathcommon.h"

class FDAPI vec4 {
private:
	friend class vec3;
	friend class vec2;
private:
	union {
		float fdata[4];
		struct {
			float _x;
			float _y;
			float _z;
			float _w;
		};
	};

	

public:
	vec4();
	//vec4(__m128 xmm);
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

	__forceinline vec4 operator-() { return vec4(-_x, -_y, -_z, -_w); }

	__forceinline float GetX() const { return _x; }
	__forceinline float GetY() const { return _y; }
	__forceinline float GetZ() const { return _z; }
	__forceinline float GetW() const { return _w; }
};
