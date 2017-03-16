#include "math.h"
#include <memory>

namespace FD {

#pragma region _vec4

//vec4::vec4(__m128 xmm) : _xmm(xmm) { }

vec4::vec4() : vec4(0, 0, 0, 0) {}

vec4::vec4(const vec2& v, float32 z, float32 w) : x(v.x), y(v.y), z(z), w(w) {}

vec4::vec4(const vec3& v, float32 w) : x(v.x), y(v.y), z(v.z), w(w) {}

vec4::vec4(float32 x, float32 y, float32 z, float32 w) : x(x), y(y), z(z), w(w) {}

vec4& vec4::Add(const vec4& v) {
	__m128 vxmm = _mm_set_ps(v.w, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_add_ps(xmm, vxmm);
	memcpy(this, xmm.m128_f32, sizeof(float32) * 4);
	return *this;
}

vec4& vec4::Add(float32 v) {
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_add_ps(xmm, _mm_set_ps(v, v, v, v));
	memcpy(this, xmm.m128_f32, sizeof(float32) * 4);
	return *this;
}

vec4& vec4::Subtract(const vec4& v) {
	__m128 vxmm = _mm_set_ps(v.w, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_sub_ps(xmm, vxmm);
	memcpy(this, xmm.m128_f32, sizeof(float32) * 4);
	return *this;
}

vec4& vec4::Subtract(float32 v) {
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_sub_ps(xmm, _mm_set_ps(v, v, v, v));
	memcpy(this, xmm.m128_f32, sizeof(float32) * 4);
	return *this;
}

vec4& vec4::Multiply(const vec4& v) {
	__m128 vxmm = _mm_set_ps(v.w, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_mul_ps(xmm, vxmm);
	memcpy(this, xmm.m128_f32, sizeof(float32) * 4);
	return *this;
}

vec4& vec4::Multiply(float32 v) {
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_mul_ps(xmm, _mm_set_ps(v, v, v, v));
	memcpy(this, xmm.m128_f32, sizeof(float32) * 4);
	return *this;
}

vec4& vec4::Divide(const vec4& v) {
	__m128 vxmm = _mm_set_ps(v.w, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_div_ps(xmm, vxmm);
	memcpy(this, xmm.m128_f32, sizeof(float32) * 4);
	return *this;
}

vec4& vec4::Divide(float32 v) {
	__m128 xmm = _mm_set_ps(w, z, y, x);
	xmm = _mm_div_ps(xmm, _mm_set_ps(v, v, v, v));
	memcpy(this, xmm.m128_f32, sizeof(float32) * 4);
	return *this;
}

#pragma endregion 

#pragma region ivec4


ivec4::ivec4() : ivec4(0, 0, 0, 0) {}

ivec4::ivec4(const ivec2& v, int32 z, int32 w) : x(v.x), y(v.y), z(z), w(w) {}

ivec4::ivec4(const ivec3& v, int32 w) : x(v.x), y(v.y), z(v.z), w(w) {}

ivec4::ivec4(int32 x, int32 y, int32 z, int32 w) : x(x), y(y), z(z), w(w) {}

ivec4& ivec4::Add(const ivec4& v) {
	__m128i vxmm = _mm_set_epi32(v.w, v.z, v.y, v.x);
	__m128i xmm = _mm_set_epi32(w, z, y, x);
	xmm = _mm_add_epi32(xmm, vxmm);
	memcpy(this, xmm.m128i_i32, sizeof(int32) * 4);
	return *this;
}

ivec4& ivec4::Add(int32 v) {
	__m128i xmm = _mm_set_epi32(w, z, y, x);
	xmm = _mm_add_epi32(xmm, _mm_set_epi32(v, v, v, v));
	memcpy(this, xmm.m128i_i32, sizeof(int32) * 4);
	return *this;
}

ivec4& ivec4::Subtract(const ivec4& v) {
	__m128i vxmm = _mm_set_epi32(v.w, v.z, v.y, v.x);
	__m128i xmm = _mm_set_epi32(w, z, y, x);
	xmm = _mm_sub_epi32(xmm, vxmm);
	memcpy(this, xmm.m128i_i32, sizeof(int32) * 4);
	return *this;
}

ivec4& ivec4::Subtract(int32 v) {
	__m128i xmm = _mm_set_epi32(w, z, y, x);
	xmm = _mm_sub_epi32(xmm, _mm_set_epi32(v, v, v, v));
	memcpy(this, xmm.m128i_i32, sizeof(int32) * 4);
	return *this;
}

ivec4& ivec4::Multiply(const ivec4& v) {
	__m256i vxmm = _mm256_set_epi32(0, v.w, 0, v.z, 0, v.y, 0, v.x);
	__m256i xmm = _mm256_set_epi32(0, w, 0, z, 0, y, 0, x);
	xmm = _mm256_mul_epi32(xmm, vxmm);
	x = xmm.m256i_i32[0];
	y = xmm.m256i_i32[2];
	z = xmm.m256i_i32[4];
	w = xmm.m256i_i32[6];
	return *this;
}

ivec4& ivec4::Multiply(int32 v) {
	__m256i xmm = _mm256_set_epi32(0, w, 0, z, 0, y, 0, x);
	xmm = _mm256_mul_epi32(xmm, _mm256_set_epi32(0, v, 0, v, 0, v, 0, v));
	x = xmm.m256i_i32[0];
	y = xmm.m256i_i32[2];
	z = xmm.m256i_i32[4];
	w = xmm.m256i_i32[6];
	return *this;
}

ivec4& ivec4::Divide(const ivec4& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
	return *this;
}

ivec4& ivec4::Divide(int32 v) {
	x /= v;
	y /= v;
	z /= v;
	w /= v;
	return *this;
}

#pragma endregion 

}