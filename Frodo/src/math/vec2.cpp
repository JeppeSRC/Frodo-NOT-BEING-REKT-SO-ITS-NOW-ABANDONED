#include "math.h"
#include <memory>

vec2::vec2(__m128 xmm) : _xmm(xmm) { }

vec2::vec2() {
	memset(&_xmm, 0, sizeof(__m128));
}

vec2::vec2(float x, float y) : _x(x), _y(y) { }

vec2& vec2::Add(const vec2& v) {
	_xmm = _mm_add_ps(_xmm, v._xmm);
	return *this;
}

vec2& vec2::Add(float v) {
	_xmm = _mm_add_ps(_xmm, _mm_set_ps(v, v, 0, 0));
	return *this;
}

vec2& vec2::Subtract(const vec2& v) {
	_xmm = _mm_sub_ps(_xmm, v._xmm);
	return *this;
}

vec2& vec2::Subtract(float v) {
	_xmm = _mm_sub_ps(_xmm, _mm_set_ps(v, v, 0, 0));
	return *this;
}

vec2& vec2::Multiply(const vec2& v) {
	_xmm = _mm_mul_ps(_xmm, v._xmm);
	return *this;
}

vec2& vec2::Multiply(float v) {
	_xmm = _mm_mul_ps(_xmm, _mm_set_ps(v, v, 0, 0));
	return *this;
}

vec2& vec2::Divide(const vec2& v) {
	_xmm = _mm_div_ps(_xmm, v._xmm);
	return *this;
}

vec2& vec2::Divide(float v) {
	_xmm = _mm_div_ps(_xmm, _mm_set_ps(v, v, 0, 0));
	return *this;
}