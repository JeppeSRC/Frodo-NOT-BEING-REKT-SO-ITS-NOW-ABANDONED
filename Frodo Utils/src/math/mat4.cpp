#include "math.h"
#include <memory>

namespace FD {

void mat4::LoadRows(__m128* xmm) const {

	xmm[0] = _mm_set_ps(m[0 + 3 * 4], m[0 + 2 * 4], m[0 + 1 * 4], m[0 + 0 * 4]);
	xmm[1] = _mm_set_ps(m[1 + 3 * 4], m[1 + 2 * 4], m[1 + 1 * 4], m[1 + 0 * 4]);
	xmm[2] = _mm_set_ps(m[2 + 3 * 4], m[2 + 2 * 4], m[2 + 1 * 4], m[2 + 0 * 4]);
	xmm[3] = _mm_set_ps(m[3 + 3 * 4], m[3 + 2 * 4], m[3 + 1 * 4], m[3 + 0 * 4]);
}

void mat4::LoadColumns(__m128* xmm) const {

	xmm[0] = _mm_set_ps(m[3 + 0 * 4], m[2 + 0 * 4], m[1 + 0 * 4], m[0 + 0 * 4]);
	xmm[1] = _mm_set_ps(m[3 + 1 * 4], m[2 + 1 * 4], m[1 + 1 * 4], m[0 + 1 * 4]);
	xmm[2] = _mm_set_ps(m[3 + 2 * 4], m[2 + 2 * 4], m[1 + 2 * 4], m[0 + 2 * 4]);
	xmm[3] = _mm_set_ps(m[3 + 3 * 4], m[2 + 3 * 4], m[1 + 3 * 4], m[0 + 3 * 4]);
}

mat4::mat4() { memset(m, 0, sizeof(m)); }

mat4::mat4(float32 diagonal) {
	memset(m, 0, sizeof(m));
	m[0 + 0 * 4] = diagonal;
	m[1 + 1 * 4] = diagonal;
	m[2 + 2 * 4] = diagonal;
	m[3 + 3 * 4] = diagonal;
}

mat4 mat4::Translate(const vec3& v) {
	mat4 tmp(1);

	tmp.m[0 + 3 * 4] = v.x;
	tmp.m[1 + 3 * 4] = v.y;
	tmp.m[2 + 3 * 4] = v.z;

	return tmp;
}

mat4 mat4::Rotate(const vec3& v) {
	mat4 x(1), y(1), z(1);

	float32 xcos = cosf((float32)FD_TO_RADIANS_F(v.x));
	float32 xsin = sinf((float32)FD_TO_RADIANS_F(v.x));
	float32 ycos = cosf((float32)FD_TO_RADIANS_F(v.y));
	float32 ysin = sinf((float32)FD_TO_RADIANS_F(v.y));
	float32 zcos = cosf((float32)FD_TO_RADIANS_F(v.z));
	float32 zsin = sinf((float32)FD_TO_RADIANS_F(v.z));

	x.m[1 + 1 * 4] = xcos; x.m[1 + 2 * 4] = -xsin;
	x.m[2 + 1 * 4] = xsin; x.m[2 + 2 * 4] = xcos;

	y.m[0 + 0 * 4] = ycos; y.m[0 + 2 * 4] = -ysin;
	y.m[2 + 0 * 4] = ysin; y.m[2 + 2 * 4] = ycos;

	z.m[0 + 0 * 4] = zcos; z.m[0 + 1 * 4] = -zsin;
	z.m[1 + 0 * 4] = zsin; z.m[1 + 1 * 4] = zcos;

	return x * y * z;
}

mat4 mat4::Scale(const vec3& v) {
	mat4 tmp(1);

	tmp.m[0 + 0 * 4] = v.x;
	tmp.m[1 + 1 * 4] = v.y;
	tmp.m[2 + 2 * 4] = v.z;

	return tmp;
}

mat4 mat4::Inverse(mat4 m) {

	float tmp[16];

	tmp[0] = m.m[5] * m.m[10] * m.m[15] -
		m.m[5] * m.m[11] * m.m[14] -
		m.m[9] * m.m[6] * m.m[15] +
		m.m[9] * m.m[7] * m.m[14] +
		m.m[13] * m.m[6] * m.m[11] -
		m.m[13] * m.m[7] * m.m[10];

	tmp[4] = -m.m[4] * m.m[10] * m.m[15] +
		m.m[4] * m.m[11] * m.m[14] +
		m.m[8] * m.m[6] * m.m[15] -
		m.m[8] * m.m[7] * m.m[14] -
		m.m[12] * m.m[6] * m.m[11] +
		m.m[12] * m.m[7] * m.m[10];

	tmp[8] = m.m[4] * m.m[9] * m.m[15] -
		m.m[4] * m.m[11] * m.m[13] -
		m.m[8] * m.m[5] * m.m[15] +
		m.m[8] * m.m[7] * m.m[13] +
		m.m[12] * m.m[5] * m.m[11] -
		m.m[12] * m.m[7] * m.m[9];

	tmp[12] = -m.m[4] * m.m[9] * m.m[14] +
		m.m[4] * m.m[10] * m.m[13] +
		m.m[8] * m.m[5] * m.m[14] -
		m.m[8] * m.m[6] * m.m[13] -
		m.m[12] * m.m[5] * m.m[10] +
		m.m[12] * m.m[6] * m.m[9];

	tmp[1] = -m.m[1] * m.m[10] * m.m[15] +
		m.m[1] * m.m[11] * m.m[14] +
		m.m[9] * m.m[2] * m.m[15] -
		m.m[9] * m.m[3] * m.m[14] -
		m.m[13] * m.m[2] * m.m[11] +
		m.m[13] * m.m[3] * m.m[10];

	tmp[5] = m.m[0] * m.m[10] * m.m[15] -
		m.m[0] * m.m[11] * m.m[14] -
		m.m[8] * m.m[2] * m.m[15] +
		m.m[8] * m.m[3] * m.m[14] +
		m.m[12] * m.m[2] * m.m[11] -
		m.m[12] * m.m[3] * m.m[10];

	tmp[9] = -m.m[0] * m.m[9] * m.m[15] +
		m.m[0] * m.m[11] * m.m[13] +
		m.m[8] * m.m[1] * m.m[15] -
		m.m[8] * m.m[3] * m.m[13] -
		m.m[12] * m.m[1] * m.m[11] +
		m.m[12] * m.m[3] * m.m[9];

	tmp[13] = m.m[0] * m.m[9] * m.m[14] -
		m.m[0] * m.m[10] * m.m[13] -
		m.m[8] * m.m[1] * m.m[14] +
		m.m[8] * m.m[2] * m.m[13] +
		m.m[12] * m.m[1] * m.m[10] -
		m.m[12] * m.m[2] * m.m[9];

	tmp[2] = m.m[1] * m.m[6] * m.m[15] -
		m.m[1] * m.m[7] * m.m[14] -
		m.m[5] * m.m[2] * m.m[15] +
		m.m[5] * m.m[3] * m.m[14] +
		m.m[13] * m.m[2] * m.m[7] -
		m.m[13] * m.m[3] * m.m[6];

	tmp[6] = -m.m[0] * m.m[6] * m.m[15] +
		m.m[0] * m.m[7] * m.m[14] +
		m.m[4] * m.m[2] * m.m[15] -
		m.m[4] * m.m[3] * m.m[14] -
		m.m[12] * m.m[2] * m.m[7] +
		m.m[12] * m.m[3] * m.m[6];

	tmp[10] = m.m[0] * m.m[5] * m.m[15] -
		m.m[0] * m.m[7] * m.m[13] -
		m.m[4] * m.m[1] * m.m[15] +
		m.m[4] * m.m[3] * m.m[13] +
		m.m[12] * m.m[1] * m.m[7] -
		m.m[12] * m.m[3] * m.m[5];

	tmp[14] = -m.m[0] * m.m[5] * m.m[14] +
		m.m[0] * m.m[6] * m.m[13] +
		m.m[4] * m.m[1] * m.m[14] -
		m.m[4] * m.m[2] * m.m[13] -
		m.m[12] * m.m[1] * m.m[6] +
		m.m[12] * m.m[2] * m.m[5];

	tmp[3] = -m.m[1] * m.m[6] * m.m[11] +
		m.m[1] * m.m[7] * m.m[10] +
		m.m[5] * m.m[2] * m.m[11] -
		m.m[5] * m.m[3] * m.m[10] -
		m.m[9] * m.m[2] * m.m[7] +
		m.m[9] * m.m[3] * m.m[6];

	tmp[7] = m.m[0] * m.m[6] * m.m[11] -
		m.m[0] * m.m[7] * m.m[10] -
		m.m[4] * m.m[2] * m.m[11] +
		m.m[4] * m.m[3] * m.m[10] +
		m.m[8] * m.m[2] * m.m[7] -
		m.m[8] * m.m[3] * m.m[6];

	tmp[11] = -m.m[0] * m.m[5] * m.m[11] +
		m.m[0] * m.m[7] * m.m[9] +
		m.m[4] * m.m[1] * m.m[11] -
		m.m[4] * m.m[3] * m.m[9] -
		m.m[8] * m.m[1] * m.m[7] +
		m.m[8] * m.m[3] * m.m[5];

	tmp[15] = m.m[0] * m.m[5] * m.m[10] -
		m.m[0] * m.m[6] * m.m[9] -
		m.m[4] * m.m[1] * m.m[10] +
		m.m[4] * m.m[2] * m.m[9] +
		m.m[8] * m.m[1] * m.m[6] -
		m.m[8] * m.m[2] * m.m[5];

	mat4 n;

	float determinant = m.m[0] * tmp[0] + m.m[1] * tmp[4] + m.m[2] * tmp[8] + m.m[3] * tmp[12];

	for (uint_t i = 0; i < 16; i++)
		n.m[i] = tmp[i] * determinant;

	return n;
}

mat4 mat4::Perspective(float32 fov, float32 aspect, float32 zNear, float32 zFar) {

	mat4 r(1);

	float32* m = r.m;

	const float32 tanHalf = tanh(fov / 2);

	m[0 + 0 * 4] = 1.0f / (tanHalf * aspect);
	m[1 + 1 * 4] = 1.0f / tanHalf;
	m[2 + 2 * 4] = zFar / (zFar - zNear);
	m[3 + 2 * 4] = 1;
	m[2 + 3 * 4] = -zNear * (zFar / (zFar - zNear));
	m[3 + 3 * 4] = 0;

	return r;
}


mat4 mat4::Orthographic(float32 left, float32 right, float32 top, float32 bottom, float32 zNear, float32 zFar) {
	mat4 r;

	float32* m = r.m;

	float w = 2.0f / (right - left);
	float h = 2.0f / (top - bottom);
	float z = 1.0f / (zFar - zNear);

	m[0 + 0 * 4] = w;
	m[1 + 1 * 4] = h;
	m[2 + 2 * 4] = z;

	m[0 + 3 * 4] = 0;
	m[1 + 3 * 4] = 0;
	m[2 + 3 * 4] = -z * zNear;
	m[3 + 3 * 4] = 1.0f;

	return r;
}

mat4 mat4::LookAt(vec3 position, vec3 target, vec3 up) {
	vec3 z = (target - position).Normalize();
	vec3 x = (up.Cross(z)).Normalize();
	vec3 y = z.Cross(x);

	mat4 m;

	m.m[0 + 0 * 4] = x.x;	m.m[0 + 1 * 4] = x.y;	m.m[0 + 2 * 4] = x.z;	m.m[0 + 3 * 4] = -(x.Dot(position));
	m.m[1 + 0 * 4] = y.x;	m.m[1 + 1 * 4] = y.y;	m.m[1 + 2 * 4] = y.z;	m.m[1 + 3 * 4] = -(y.Dot(position));
	m.m[2 + 0 * 4] = z.x;	m.m[2 + 1 * 4] = z.y;	m.m[2 + 2 * 4] = z.z;	m.m[2 + 3 * 4] = -(z.Dot(position));

	m.m[3 + 0 * 4] = 0;	m.m[3 + 1 * 4] = 0; m.m[3 + 2 * 4] = 0; m.m[3 + 3 * 4] = 1;

	return m;
}

mat4 mat4::Transpose(mat4 m) {
	float tmp[16];
	memcpy(tmp, m.m, sizeof(m));

	for (uint32 y = 0; y < 4; y++) {
		for (uint32 x = 0; x < 4; x++) {
			m.m[y + x * 4] = tmp[x + y * 4];
		}
	}

	return m;
}

mat4 mat4::operator*(const mat4& r) const {
	mat4 tmp;
	__m128 col[4];
	__m128 rows[4];

	r.LoadColumns(col);
	LoadRows(rows);

	for (int32 y = 0; y < 4; y++) {
		for (int32 x = 0; x < 4; x++) {
			__m128 res = _mm_mul_ps(rows[x], col[y]);
			tmp.m[x + y * 4] = res.m128_f32[0] + res.m128_f32[1] + res.m128_f32[2] + res.m128_f32[3];
		}
	}


	return tmp;
}

vec4 mat4::operator*(const vec4& v) const {
	__m128 vec[4];
	__m128 col[4];

	vec[0] = _mm_set_ps(v.x, v.x, v.x, v.x);
	vec[1] = _mm_set_ps(v.y, v.y, v.y, v.y);
	vec[2] = _mm_set_ps(v.z, v.z, v.z, v.z);
	vec[3] = _mm_set_ps(v.w, v.w, v.w, v.w);

	LoadColumns(col);

	__m128 res = _mm_mul_ps(vec[0], col[0]);

	for (int32 i = 1; i < 4; i++)
		res = _mm_fmadd_ps(vec[i], col[i], res);

	return vec4(res.m128_f32[0], res.m128_f32[1], res.m128_f32[2], res.m128_f32[3]);
}

vec3 mat4::operator*(const vec3& v) const {
	__m128 vec[4];
	__m128 col[4];

	vec[0] = _mm_set_ps(v.x, v.x, v.x, v.x);
	vec[1] = _mm_set_ps(v.y, v.y, v.y, v.y);
	vec[2] = _mm_set_ps(v.z, v.z, v.z, v.z);
	vec[3] = _mm_set_ps(1, 1, 1, 1);

	LoadColumns(col);

	__m128 res = _mm_mul_ps(vec[0], col[0]);

	for (int32 i = 1; i < 4; i++)
		res = _mm_fmadd_ps(vec[i], col[i], res);

	return vec3(res.m128_f32[0], res.m128_f32[1], res.m128_f32[2]);
}

}