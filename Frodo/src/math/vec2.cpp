#include "math.h"
#include <memory>

namespace FD {

#pragma region vec2

vec2::vec2() { x = 0; y = 0; }
vec2::vec2(float32 x, float32 y) { this->x = x; this->y = y; }

vec2& vec2::Add(const vec2& v) {
	__m128 vxmm = _mm_set_ps(0, 0, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_add_ps(xmm, vxmm);
	memcpy(this, xmm.m128_f32, sizeof(float32) * 2);
	return *this;
}

vec2& vec2::Add(float32 v) {
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_add_ps(xmm, _mm_set_ps(0, 0, v, v));
	memcpy(this, xmm.m128_f32, sizeof(float32) * 2);
	return *this;
}

vec2& vec2::Subtract(const vec2& v) {
	__m128 vxmm = _mm_set_ps(0, 0, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_sub_ps(xmm, vxmm);
	memcpy(this, xmm.m128_f32, sizeof(float32) * 2);
	return *this;
}

vec2& vec2::Subtract(float32 v) {
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_sub_ps(xmm, _mm_set_ps(0, 0, v, v));
	memcpy(this, xmm.m128_f32, sizeof(float32) * 2);
	return *this;
}

vec2& vec2::Multiply(const vec2& v) {
	__m128 vxmm = _mm_set_ps(0, 0, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_mul_ps(xmm, vxmm);
	memcpy(this, xmm.m128_f32, sizeof(float32) * 2);
	return *this;
}

vec2& vec2::Multiply(float32 v) {
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_mul_ps(xmm, _mm_set_ps(0, 0, v, v));
	memcpy(this, xmm.m128_f32, sizeof(float32) * 2);
	return *this;
}

vec2& vec2::Divide(const vec2& v) {
	__m128 vxmm = _mm_set_ps(0, 0, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_div_ps(xmm, vxmm);
	memcpy(this, xmm.m128_f32, sizeof(float32) * 2);
	return *this;
}

vec2& vec2::Divide(float32 v) {
	__m128 xmm = _mm_set_ps(0, 0, y, x);
	xmm = _mm_div_ps(xmm, _mm_set_ps(1, 1, v, v));
	memcpy(this, xmm.m128_f32, sizeof(float32) * 2);
	return *this;
}

#pragma endregion 


#pragma region ivec2

ivec2::ivec2() { x = 0; y = 0; }
ivec2::ivec2(int32 x, int32 y) { this->x = x; this->y = y; }

ivec2& ivec2::Add(const ivec2& v) {
	__m128i vxmm = _mm_set_epi32(0, 0, v.y, v.x);
	__m128i xmm = _mm_set_epi32(0, 0, y, x);
	xmm = _mm_add_epi32(xmm, vxmm);
	memcpy(this, xmm.m128i_i32, sizeof(int32) * 2);
	return *this;
}

ivec2& ivec2::Add(int32 v) {
	__m128i xmm = _mm_set_epi32(0, 0, y, x);
	xmm = _mm_add_epi32(xmm, _mm_set_epi32(0, 0, v, v));
	memcpy(this, xmm.m128i_i32, sizeof(int32) * 2);
	return *this;
}

ivec2& ivec2::Subtract(const ivec2& v) {
	__m128i vxmm = _mm_set_epi32(0, 0, v.y, v.x);
	__m128i xmm = _mm_set_epi32(0, 0, y, x);
	xmm = _mm_sub_epi32(xmm, vxmm);
	memcpy(this, xmm.m128i_i32, sizeof(int32) * 2);
	return *this;
}

ivec2& ivec2::Subtract(int32 v) {
	__m128i xmm = _mm_set_epi32(0, 0, y, x);
	xmm = _mm_sub_epi32(xmm, _mm_set_epi32(0, 0, v, v));
	memcpy(this, xmm.m128i_i32, sizeof(int32) * 2);
	return *this;
}

ivec2& ivec2::Multiply(const ivec2& v) {
	__m128i vxmm = _mm_set_epi32(0, v.y, 0, v.x);
	__m128i xmm = _mm_set_epi32(0, y, 0, x);
	xmm = _mm_mul_epi32(xmm, vxmm);
	x = xmm.m128i_i32[0];
	y = xmm.m128i_i32[2];
	return *this;
}

ivec2& ivec2::Multiply(int32 v) {
	__m128i xmm = _mm_set_epi32(0, y, 0, x);
	xmm = _mm_mul_epi32(xmm, _mm_set_epi32(0, v, 0, v));
	x = xmm.m128i_i32[0];
	y = xmm.m128i_i32[2];
	return *this;
}

ivec2& ivec2::Divide(const ivec2& v) {
	x /= v.x;
	y /= v.y;
	return *this;
}

ivec2& ivec2::Divide(int32 v) {
	x /= v;
	y /= v;
	return *this;
}

#pragma endregion

}