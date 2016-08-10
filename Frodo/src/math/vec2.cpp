#include "math.h"
#include <memory>

//vec2::vec2(__m128 xmm) : _xmm(xmm) { }

vec2::vec2() : vec2(0, 0) {}

vec2::vec2(float x, float y) : x(x), y(y), z(0.0f), w(0.0f) { }

vec2& vec2::Add(const vec2& v) {
	__m128 vxmm = _mm_set_ps(0, 0, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_add_ps(xmm, vxmm);
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec2& vec2::Add(float v) {
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_add_ps(xmm, _mm_set_ps(0, 0, v, v));
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec2& vec2::Subtract(const vec2& v) {
	__m128 vxmm = _mm_set_ps(0, 0, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_sub_ps(xmm, vxmm);
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec2& vec2::Subtract(float v) {
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_sub_ps(xmm, _mm_set_ps(0, 0, v, v));
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec2& vec2::Multiply(const vec2& v) {
	__m128 vxmm = _mm_set_ps(0, 0, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_mul_ps(xmm, vxmm);
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec2& vec2::Multiply(float v) {
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_mul_ps(xmm, _mm_set_ps(0, 0, v, v));
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec2& vec2::Divide(const vec2& v) {
	__m128 vxmm = _mm_set_ps(0, 0, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_div_ps(xmm, vxmm);
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec2& vec2::Divide(float v) {
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_div_ps(xmm, _mm_set_ps(1, 1, v, v));
	_mm_storeu_ps(fdata, xmm);
	return *this;
}