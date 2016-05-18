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