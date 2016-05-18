#include "math.h"
#include <memory>

void mat3::LoadRows(__m128* xmm) const {

	xmm[0] = _mm_set_ps(0, m[0 + 2 * 3], m[0 + 1 * 3], m[0 + 0 * 3]);
	xmm[1] = _mm_set_ps(0, m[1 + 2 * 3], m[1 + 1 * 3], m[1 + 0 * 3]);
	xmm[2] = _mm_set_ps(0, m[2 + 2 * 3], m[2 + 1 * 3], m[2 + 0 * 3]);
}

void mat3::LoadColumns(__m128* xmm) const {

	xmm[0] = _mm_set_ps(0, m[2 + 0 * 3], m[1 + 0 * 3], m[0 + 0 * 3]);
	xmm[1] = _mm_set_ps(0, m[2 + 1 * 3], m[1 + 1 * 3], m[0 + 1 * 3]);
	xmm[2] = _mm_set_ps(0, m[2 + 2 * 3], m[1 + 2 * 3], m[0 + 2 * 3]);
}

mat3::mat3() { memset(m, 0, sizeof(m)); }

mat3::mat3(float diagonal) {
	memset(m, 0, sizeof(m));
	m[0 + 0 * 3] = diagonal;
	m[1 + 1 * 3] = diagonal;
	m[2 + 2 * 3] = diagonal;
}

mat3 mat3::Rotate(const vec3& v) {
	mat3 x(1), y(1), z(1);

	float xcos = cosf((float)FD_TO_RADIANS(v.GetX()));
	float xsin = sinf((float)FD_TO_RADIANS(v.GetX()));
	float ycos = cosf((float)FD_TO_RADIANS(v.GetY()));
	float ysin = sinf((float)FD_TO_RADIANS(v.GetY()));
	float zcos = cosf((float)FD_TO_RADIANS(v.GetZ()));
	float zsin = sinf((float)FD_TO_RADIANS(v.GetZ()));

	x.m[1 + 1 * 3] = xcos;x.m[1 + 2 * 3] = -xsin;
	x.m[2 + 1 * 3] = xsin;x.m[2 + 2 * 3] = xcos;

	y.m[0 + 0 * 3] = ycos;y.m[0 + 2 * 3] = -ysin;
	y.m[2 + 0 * 3] = ysin;y.m[2 + 2 * 3] = ycos;

	z.m[0 + 0 * 3] = zcos;z.m[0 + 1 * 3] = -zsin;
	z.m[1 + 0 * 3] = zsin;z.m[1 + 1 * 3] = zcos;

	return x * y * z;
}

mat3 mat3::Scale(const vec3& v) {
	mat3 tmp(1);

	tmp.m[0 + 0 * 3] = v.GetX();
	tmp.m[1 + 1 * 3] = v.GetY();
	tmp.m[2 + 2 * 3] = v.GetZ();

	return tmp;
}

mat3 mat3::operator*(const mat3& r) {
	mat3 tmp;
	__m128 col[3];
	__m128 rows[3];

	r.LoadColumns(col);
	LoadRows(rows);

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			__m128 res = _mm_mul_ps(rows[x], col[y]);
			tmp.m[x + y * 3] = res.m128_f32[0] + res.m128_f32[1] + res.m128_f32[2];
		}
	}

	return tmp;
}

vec3 mat3::operator*(const vec3& v) {
	__m128 vec[3];
	__m128 col[3];

	vec[0] = _mm_set_ps(0, v.GetX(), v.GetX(), v.GetX());
	vec[1] = _mm_set_ps(0, v.GetY(), v.GetY(), v.GetY());
	vec[2] = _mm_set_ps(0, v.GetZ(), v.GetZ(), v.GetZ());

	LoadColumns(col);

	__m128 res = _mm_mul_ps(vec[0], col[0]);

	for (int i = 1; i < 3; i++)
		res = _mm_fmadd_ps(vec[i], col[i], res);

	return vec3(res.m128_f32[0], res.m128_f32[1], res.m128_f32[2]);
}