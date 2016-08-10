#include "math.h"
#include <memory>

//vec4::vec4(__m128 xmm) : _xmm(xmm) { }

vec4::vec4() : vec4(0, 0, 0, 0) { }

vec4::vec4(const vec2& v, float z, float w) : x(v.x), y(v.y), z(z), w(w) { }

vec4::vec4(const vec3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}

vec4::vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }

vec4& vec4::Add(const vec4& v) {
	__m128 vxmm = _mm_set_ps(v.w, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_add_ps(xmm, vxmm);
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec4& vec4::Add(float v) {
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_add_ps(xmm, _mm_set_ps(v, v, v, v));
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec4& vec4::Subtract(const vec4& v) {
	__m128 vxmm = _mm_set_ps(v.w, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_sub_ps(xmm, vxmm);
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec4& vec4::Subtract(float v) {
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_sub_ps(xmm, _mm_set_ps(v, v, v, v));
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec4& vec4::Multiply(const vec4& v) {
	__m128 vxmm = _mm_set_ps(v.w, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_mul_ps(xmm, vxmm);
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec4& vec4::Multiply(float v) {
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_mul_ps(xmm, _mm_set_ps(v, v, v, v));
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec4& vec4::Divide(const vec4& v) {
	__m128 vxmm = _mm_set_ps(v.w, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_div_ps(xmm, vxmm);
	_mm_storeu_ps(fdata, xmm);
	return *this;
}

vec4& vec4::Divide(float v) {
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_div_ps(xmm, _mm_set_ps(v, v, v, v));
	_mm_storeu_ps(fdata, xmm);
	return *this;
}