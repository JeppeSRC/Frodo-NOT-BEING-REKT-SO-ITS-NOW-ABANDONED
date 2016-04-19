#include "math.h"
#include <memory>

vec4::vec4(__m128 xmm) : _xmm(xmm) { }

vec4::vec4() {
	memset(&_xmm, 0, sizeof(__m128));
}

vec4::vec4(const vec2& v, float z, float w) : _x(v.x()), _y(v.y()), _z(z), _w(w) { }

vec4::vec4(const vec3& v, float w) : _x(v.x()), _y(v.y()), _z(v.z()), _w(w) {}

vec4::vec4(float x, float y, float z, float w) : _x(x), _y(y), _z(z), _w(w) { }

vec4& vec4::Add(const vec4& v) {
	_xmm = _mm_add_ps(_xmm, v._xmm);
	return *this;
}

vec4& vec4::Add(float v) {
	_xmm = _mm_add_ps(_xmm, _mm_set_ps(v, v, v, v));
	return *this;
}

vec4& vec4::Subtract(const vec4& v) {
	_xmm = _mm_sub_ps(_xmm, v._xmm);
	return *this;
}

vec4& vec4::Subtract(float v) {
	_xmm = _mm_sub_ps(_xmm, _mm_set_ps(v, v, v, v));
	return *this;
}

vec4& vec4::Multiply(const vec4& v) {
	_xmm = _mm_mul_ps(_xmm, v._xmm);
	return *this;
}

vec4& vec4::Multiply(float v) {
	_xmm = _mm_mul_ps(_xmm, _mm_set_ps(v, v, v, v));
	return *this;
}

vec4& vec4::Divide(const vec4& v) {
	_xmm = _mm_div_ps(_xmm, v._xmm);
	return *this;
}

vec4& vec4::Divide(float v) {
	_xmm = _mm_div_ps(_xmm, _mm_set_ps(v, v, v, v));
	return *this;
}