#include "math.h"
#include <memory>

vec3::vec3(__m128 xmm) : _xmm(xmm) { }

vec3::vec3() {
	memset(&_xmm, 0, sizeof(__m128));
}

vec3::vec3(const vec2& v) : _x(v.x()), _y(v.y()), _z(0) { }

vec3::vec3(float x, float y, float z) : _x(x), _y(y), _z(z) { }

vec3& vec3::Add(const vec3& v) {
	_xmm = _mm_add_ps(_xmm, v._xmm);
	return *this;
}

vec3& vec3::Add(float v) {
	_xmm = _mm_add_ps(_xmm, _mm_set_ps(0, v, v, v));
	return *this;
}

vec3& vec3::Subtract(const vec3& v) {
	_xmm = _mm_sub_ps(_xmm, v._xmm);
	return *this;
}

vec3& vec3::Subtract(float v) {
	_xmm = _mm_sub_ps(_xmm, _mm_set_ps(0, v, v, v));
	return *this;
}

vec3& vec3::Multiply(const vec3& v) {
	_xmm = _mm_mul_ps(_xmm, v._xmm);
	return *this;
}

vec3& vec3::Multiply(float v) {
	_xmm = _mm_mul_ps(_xmm, _mm_set_ps(0, v, v, v));
	return *this;
}

vec3& vec3::Divide(const vec3& v) {
	_xmm = _mm_div_ps(_xmm, v._xmm);
	return *this;
}

vec3& vec3::Divide(float v) {
	_xmm = _mm_div_ps(_xmm, _mm_set_ps(0, v, v, v));
	return *this;
}