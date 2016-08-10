#include "math.h"
#include <memory>

//vec3::vec3(__m128 xmm) : _xmm(xmm) { }

vec3::vec3() : vec3(0, 0, 0) { }

vec3::vec3(const vec2& v, float z) : vec3(v.x, v.y, z) { }

vec3::vec3(float x, float y, float z) : x(x), y(y), z(z), w(0.0f) { }

vec3& vec3::Add(const vec3& v) {
	__m128 vxmm = _mm_set_ps(0, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_add_ps(xmm, vxmm);
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec3& vec3::Add(float v) {
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_add_ps(xmm, _mm_set_ps(0, v, v, v));
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec3& vec3::Subtract(const vec3& v) {
	__m128 vxmm = _mm_set_ps(0, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_sub_ps(xmm, vxmm);
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec3& vec3::Subtract(float v) {
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_sub_ps(xmm, _mm_set_ps(0, v, v, v));
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec3& vec3::Multiply(const vec3& v) {
	__m128 vxmm = _mm_set_ps(0, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_mul_ps(xmm, vxmm);
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec3& vec3::Multiply(float v) {
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_mul_ps(xmm, _mm_set_ps(0, v, v, v));
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec3& vec3::Divide(const vec3& v) {
	__m128 vxmm = _mm_set_ps(0, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_div_ps(xmm, vxmm);
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec3& vec3::Divide(float v) {
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_div_ps(xmm, _mm_set_ps(1, v, v, v));
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

float vec3::Dot(const vec3& v) {
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_mul_ps(xmm, _mm_set_ps(0, v.z, v.y, v.x));
	return xmm.m128_f32[0] + xmm.m128_f32[1] + xmm.m128_f32[2];
}

vec3 vec3::Cross(const vec3& v) {

	float _x = y * v.z - z * v.y;
	float _y = z * v.x - x * v.z;
	float _z = x * v.y - y * v.x;

	return vec3(_x, _y, _z);
}

vec3& vec3::RotateX(float angle) {

	float a = (float)FD_TO_RADIANS(angle);

	float _y = y * cosf(a) - z * sinf(a);
	float _z = z * cosf(a) + y * sinf(a);

	y = _y;
	z = _z;

	return *this;
}

vec3& vec3::RotateY(float angle) {

	float a = (float)FD_TO_RADIANS(angle);

	float _z = z * cosf(a) - x * sinf(a);
	float _x = x * cosf(a) + z * sinf(a);

	x = _x;
	z = _z;

	return *this;
}

vec3& vec3::RotateZ(float angle) {

	float a = (float)FD_TO_RADIANS(angle);

	float _x = x * cosf(a) - y * sinf(a);
	float _y = y * cosf(a) + x * sinf(a);

	x = _x;
	y = _y;

	return *this;
}

