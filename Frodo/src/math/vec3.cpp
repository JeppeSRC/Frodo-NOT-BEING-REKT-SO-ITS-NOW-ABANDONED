#include "math.h"
#include <core/log.h>
#include <memory>

namespace FD {

#pragma region vec3

//vec3::vec3(__m128 xmm) : _xmm(xmm) { }

vec3::vec3() { x = 0; y = 0; z = 0; }
vec3::vec3(const vec2& v, float32 z) : vec3(v.x, v.y, z) {}
vec3::vec3(float32 x, float32 y, float32 z) { this->x = x; this->y = y; this->z = z; }

vec3& vec3::Add(const vec3& v) {
	__m128 vxmm = _mm_set_ps(0, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_add_ps(xmm, vxmm);
	memcpy(this, (void*)xmm.m128_f32, sizeof(int32) * 3);
	return *this;
}

vec3& vec3::Add(float32 v) {
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_add_ps(xmm, _mm_set_ps(0, v, v, v));
	memcpy(this, (void*)xmm.m128_f32, sizeof(int32) * 3);
	return *this;
}

vec3& vec3::Subtract(const vec3& v) {
	__m128 vxmm = _mm_set_ps(0, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_sub_ps(xmm, vxmm);
	memcpy(this, (void*)xmm.m128_f32, sizeof(int32) * 3);
	return *this;
}

vec3& vec3::Subtract(float32 v) {
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_sub_ps(xmm, _mm_set_ps(0, v, v, v));
	memcpy(this, (void*)xmm.m128_f32, sizeof(int32) * 3);
	return *this;
}

vec3& vec3::Multiply(const vec3& v) {
	__m128 vxmm = _mm_set_ps(0, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_mul_ps(xmm, vxmm);
	memcpy(this, (void*)xmm.m128_f32, sizeof(int32) * 3);
	return *this;
}

vec3& vec3::Multiply(float32 v) {
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_mul_ps(xmm, _mm_set_ps(0, v, v, v));
	memcpy(this, (void*)xmm.m128_f32, sizeof(int32) * 3);
	return *this;
}

vec3& vec3::Divide(const vec3& v) {
	__m128 vxmm = _mm_set_ps(0, v.z, v.y, v.x);
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_div_ps(xmm, vxmm);
	memcpy(this, (void*)xmm.m128_f32, sizeof(int32) * 3);
	return *this;
}

vec3& vec3::Divide(float32 v) {
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_div_ps(xmm, _mm_set_ps(1, v, v, v));
	memcpy(this, (void*)xmm.m128_f32, sizeof(int32) * 3);
	return *this;
}

float32 vec3::Dot(const vec3& v) {
	__m128 xmm = _mm_set_ps(0, z, y, x);
	xmm = _mm_mul_ps(xmm, _mm_set_ps(0, v.z, v.y, v.x));
	return xmm.m128_f32[0] + xmm.m128_f32[1] + xmm.m128_f32[2];
}

vec3 vec3::Cross(const vec3& v) {

	float32 _x = y * v.z - z * v.y;
	float32 _y = z * v.x - x * v.z;
	float32 _z = x * v.y - y * v.x;

	return vec3(_x, _y, _z);
}

vec3& vec3::RotateX(float32 angle) {

	float32 a = (float32)FD_TO_RADIANS_F(angle);

	float32 _y = y * cosf(a) - z * sinf(a);
	float32 _z = z * cosf(a) + y * sinf(a);

	y = _y;
	z = _z;

	return *this;
}

vec3& vec3::RotateY(float32 angle) {

	float32 a = (float32)FD_TO_RADIANS_F(angle);

	float32 _z = z * cosf(a) - x * sinf(a);
	float32 _x = x * cosf(a) + z * sinf(a);

	x = _x;
	z = _z;

	return *this;
}

vec3& vec3::RotateZ(float32 angle) {

	float32 a = (float32)FD_TO_RADIANS_F(angle);

	float32 _x = x * cosf(a) - y * sinf(a);
	float32 _y = y * cosf(a) + x * sinf(a);

	x = _x;
	y = _y;

	return *this;
}

bool vec3::operator==(const vec3& v) const {
	return FLOAT32_CMP(x, v.x) && FLOAT32_CMP(y, v.y) && FLOAT32_CMP(z, v.z);
}

bool vec3::operator!=(const vec3& v) const {
	return !(FLOAT32_CMP(x, v.x) && FLOAT32_CMP(y, v.y) && FLOAT32_CMP(z, v.z));
}

#pragma endregion

#pragma region ivec3

ivec3::ivec3() { x = 0; y = 0; z = 0; }
ivec3::ivec3(const ivec2& v, int32 z) : ivec3(v.x, v.y, z) {}
ivec3::ivec3(int32 x, int32 y, int32 z) { this->x = x; this->y = y; this->z = z; }

ivec3& ivec3::Add(const ivec3& v) {
	__m128i vxmm = _mm_set_epi32(0, v.z, v.y, v.x);
	__m128i xmm = _mm_set_epi32(0, z, y, x);
	xmm = _mm_add_epi32(xmm, vxmm);
	memcpy(this, (void*)xmm.m128i_i32, sizeof(int32) * 3);
	return *this;
}

ivec3& ivec3::Add(int32 v) {
	__m128i xmm = _mm_set_epi32(0, z, y, x);
	xmm = _mm_add_epi32(xmm, _mm_set_epi32(0, v, v, v));
	memcpy(this, (void*)xmm.m128i_u32, sizeof(int32) * 3);
	return *this;
}

ivec3& ivec3::Subtract(const ivec3& v) {
	__m128i vxmm = _mm_set_epi32(0, v.z, v.y, v.x);
	__m128i xmm = _mm_set_epi32(0, z, y, x);
	xmm = _mm_sub_epi32(xmm, vxmm);
	memcpy(this, (void*)xmm.m128i_u32, sizeof(int32) * 3);
	return *this;
}

ivec3& ivec3::Subtract(int32 v) {
	__m128i xmm = _mm_set_epi32(0, z, y, x);
	xmm = _mm_sub_epi32(xmm, _mm_set_epi32(0, v, v, v));
	memcpy(this, (void*)xmm.m128i_u32, sizeof(int32) * 3);
	return *this;
}

ivec3& ivec3::Multiply(const ivec3& v) {
	__m256i vxmm = _mm256_set_epi32(0, 0, 0, v.z, 0, v.y, 0, v.x);
	__m256i xmm = _mm256_set_epi32(0, 0, 0, z, 0, y, 0, x);
	xmm = _mm256_mul_epi32(xmm, vxmm);
	x = xmm.m256i_i32[0];
	y = xmm.m256i_i32[2];
	z = xmm.m256i_i32[4];
	return *this;
}

ivec3& ivec3::Multiply(int32 v) {
	__m256i xmm = _mm256_set_epi32(0, 0, 0, z, 0, y, 0, x);
	xmm = _mm256_mul_epi32(xmm, _mm256_set_epi32(0, 0, 0, v, 0, v, 0, v));
	x = xmm.m256i_i32[0];
	y = xmm.m256i_i32[2];
	z = xmm.m256i_i32[4];
	return *this;
}

ivec3& ivec3::Divide(const ivec3& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

ivec3& ivec3::Divide(int32 v) {
	x /= v;
	y /= v;
	z /= v;
	return *this;
}

bool ivec3::operator==(const ivec3& v) const {
	return x == v.x && y == v.y && z == v.z;
}

bool ivec3::operator!=(const ivec3& v) const {
	return !(x == v.x && y == v.y && z == v.z);
}

#pragma endregion

}