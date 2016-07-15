#include "math.h"
#include <memory>

//vec3::vec3(__m128 xmm) : _xmm(xmm) { }

vec3::vec3() : vec3(0, 0, 0) { }

vec3::vec3(const vec2& v, float z) :vec3(v.GetX(), v.GetY(), z) { }

vec3::vec3(float x, float y, float z) : _x(x), _y(y), _z(z), _w(0.0f) { }

vec3& vec3::Add(const vec3& v) {
	__m128 vxmm = _mm_set_ps(0, v._z, v._y, v._x);
	__m128 xmm = _mm_set_ps(0, _z, _y, _x);
	xmm = _mm_add_ps(xmm, vxmm);
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec3& vec3::Add(float v) {
	__m128 xmm = _mm_set_ps(0, _z, _y, _x);
	xmm = _mm_add_ps(xmm, _mm_set_ps(0, v, v, v));
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec3& vec3::Subtract(const vec3& v) {
	__m128 vxmm = _mm_set_ps(0, v._z, v._y, v._x);
	__m128 xmm = _mm_set_ps(0, _z, _y, _x);
	xmm = _mm_sub_ps(xmm, vxmm);
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec3& vec3::Subtract(float v) {
	__m128 xmm = _mm_set_ps(0, _z, _y, _x);
	xmm = _mm_sub_ps(xmm, _mm_set_ps(0, v, v, v));
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec3& vec3::Multiply(const vec3& v) {
	__m128 vxmm = _mm_set_ps(0, v._z, v._y, v._x);
	__m128 xmm = _mm_set_ps(0, _z, _y, _x);
	xmm = _mm_mul_ps(xmm, vxmm);
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec3& vec3::Multiply(float v) {
	__m128 xmm = _mm_set_ps(0, _z, _y, _x);
	xmm = _mm_mul_ps(xmm, _mm_set_ps(0, v, v, v));
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec3& vec3::Divide(const vec3& v) {
	__m128 vxmm = _mm_set_ps(0, v._z, v._y, v._x);
	__m128 xmm = _mm_set_ps(0, _z, _y, _x);
	xmm = _mm_div_ps(xmm, vxmm);
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec3& vec3::Divide(float v) {
	__m128 xmm = _mm_set_ps(0, _z, _y, _x);
	xmm = _mm_div_ps(xmm, _mm_set_ps(1, v, v, v));
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

float vec3::Dot(const vec3& v) {
	__m128 xmm = _mm_set_ps(0, _z, _y, _x);
	xmm = _mm_mul_ps(xmm, _mm_set_ps(0, v._z, v._y, v._x));
	return xmm.m128_f32[0] + xmm.m128_f32[1] + xmm.m128_f32[2];
}

vec3 vec3::Cross(const vec3& v) {

	float x = _y * v._z - _z * v._y;
	float y = _z * v._x - _x * v._z;
	float z = _x * v._y - _y * v._x;

	return vec3(x, y, z);
}

vec3& vec3::RotateX(float angle) {

	float a = (float)FD_TO_RADIANS(angle);

	float y = _y * cosf(a) - _z * sinf(a);
	float z = _z * cosf(a) + _y * sinf(a);

	_y = y;
	_z = z;

	return *this;
}

vec3& vec3::RotateY(float angle) {

	float a = (float)FD_TO_RADIANS(angle);

	float z = _z * cosf(a) - _x * sinf(a);
	float x = _x * cosf(a) + _z * sinf(a);

	_x = x;
	_z = z;

	return *this;
}

vec3& vec3::RotateZ(float angle) {

	float a = (float)FD_TO_RADIANS(angle);

	float x = _x * cosf(a) - _y * sinf(a);
	float y = _y * cosf(a) + _x * sinf(a);

	_x = x;
	_y = y;

	return *this;
}

