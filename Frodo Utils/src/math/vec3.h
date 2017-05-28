#pragma once
#include "mathcommon.h"

namespace FD {

class FDAPI vec3 {
private:
	friend class vec2;
public:
	float32 x;
	float32 y;
	float32 z;


public:
	vec3();
	vec3(const vec2& v, float32 z);
	vec3(float32 x, float32 y, float32 z);

	vec3& Add(const vec3& v);
	vec3& Add(float32 v);
	vec3& Subtract(const vec3& v);
	vec3& Subtract(float32 v);
	vec3& Multiply(const vec3& v);
	vec3& Multiply(float32 v);
	vec3& Divide(const vec3& v);
	vec3& Divide(float32 v);
	vec3& Normalize();


	float32 Length() const;
	float32 LengthSqrt() const;

	float32 Dot(const vec3& v) const;
	vec3 Cross(const vec3& v) const;

	vec3& RotateX(float32 angle);
	vec3& RotateY(float32 angle);
	vec3& RotateZ(float32 angle);

	__forceinline friend vec3 operator+(const vec3& l, const vec3& r) { return vec3(l).Add(r); }
	__forceinline friend vec3 operator-(const vec3& l, const vec3& r) { return vec3(l).Subtract(r); }
	__forceinline friend vec3 operator*(const vec3& l, const vec3& r) { return vec3(l).Multiply(r); }
	__forceinline friend vec3 operator/(const vec3& l, const vec3& r) { return vec3(l).Divide(r); }

	__forceinline friend vec3 operator+(const vec3& l, float32 r) { return vec3(l).Add(r); }
	__forceinline friend vec3 operator-(const vec3& l, float32 r) { return vec3(l).Subtract(r); }
	__forceinline friend vec3 operator*(const vec3& l, float32 r) { return vec3(l).Multiply(r); }
	__forceinline friend vec3 operator/(const vec3& l, float32 r) { return vec3(l).Divide(r); }

	__forceinline void operator+=(const vec3& v) { Add(v); }
	__forceinline void operator-=(const vec3& v) { Subtract(v); }
	__forceinline void operator*=(const vec3& v) { Multiply(v); }
	__forceinline void operator/=(const vec3& v) { Divide(v); }

	__forceinline void operator+=(float32 v) { Add(v); }
	__forceinline void operator-=(float32 v) { Subtract(v); }
	__forceinline void operator*=(float32 v) { Multiply(v); }
	__forceinline void operator/=(float32 v) { Divide(v); }

	bool operator==(const vec3& v) const;
	bool operator!=(const vec3& v) const;

	__forceinline vec3 operator-() { return vec3(-x, -y, -z); }
};


class FDAPI ivec3 {
private:
	friend class ivec2;
public:
	int32 x;
	int32 y;
	int32 z;


public:
	ivec3();
	ivec3(const ivec2& v, int32 z);
	ivec3(int32 x, int32 y, int32 z);

	ivec3& Add(const ivec3& v);
	ivec3& Add(int32 v);
	ivec3& Subtract(const ivec3& v);
	ivec3& Subtract(int32 v);
	ivec3& Multiply(const ivec3& v);
	ivec3& Multiply(int32 v);
	ivec3& Divide(const ivec3& v);
	ivec3& Divide(int32 v);

	__forceinline friend ivec3 operator+(const ivec3& l, const ivec3& r) { return ivec3(l).Add(r); }
	__forceinline friend ivec3 operator-(const ivec3& l, const ivec3& r) { return ivec3(l).Subtract(r); }
	__forceinline friend ivec3 operator*(const ivec3& l, const ivec3& r) { return ivec3(l).Multiply(r); }
	__forceinline friend ivec3 operator/(const ivec3& l, const ivec3& r) { return ivec3(l).Divide(r); }

	__forceinline friend ivec3 operator+(const ivec3& l, int32 r) { return ivec3(l).Add(r); }
	__forceinline friend ivec3 operator-(const ivec3& l, int32 r) { return ivec3(l).Subtract(r); }
	__forceinline friend ivec3 operator*(const ivec3& l, int32 r) { return ivec3(l).Multiply(r); }
	__forceinline friend ivec3 operator/(const ivec3& l, int32 r) { return ivec3(l).Divide(r); }

	__forceinline void operator+=(const ivec3& v) { Add(v); }
	__forceinline void operator-=(const ivec3& v) { Subtract(v); }
	__forceinline void operator*=(const ivec3& v) { Multiply(v); }
	__forceinline void operator/=(const ivec3& v) { Divide(v); }

	__forceinline void operator+=(int32 v) { Add(v); }
	__forceinline void operator-=(int32 v) { Subtract(v); }
	__forceinline void operator*=(int32 v) { Multiply(v); }
	__forceinline void operator/=(int32 v) { Divide(v); }

	bool operator==(const ivec3& v) const;
	bool operator!=(const ivec3& v) const;

	__forceinline ivec3 operator-() { return ivec3(-x, -y, -z); }
};

}