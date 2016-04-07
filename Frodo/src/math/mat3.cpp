#include "math.h"
#include <memory>

mat3::mat3() { memset(m, 0, sizeof(m)); }

mat3::mat3(float diagonal) {
	memset(m, 0, sizeof(m));
	m[0 + 0 * 4] = diagonal;
	m[1 + 1 * 4] = diagonal;
	m[2 + 2 * 4] = diagonal;
}

mat3 mat3::Rotate(const vec3& v) {
	mat3 x(1), y(1), z(1);

	float xcos = cosf((float)FD_TO_RADIANS(v.x()));
	float xsin = sinf((float)FD_TO_RADIANS(v.x()));
	float ycos = cosf((float)FD_TO_RADIANS(v.y()));
	float ysin = sinf((float)FD_TO_RADIANS(v.y()));
	float zcos = cosf((float)FD_TO_RADIANS(v.z()));
	float zsin = sinf((float)FD_TO_RADIANS(v.z()));

	x.m[1 + 1 * 4] = xcos;x.m[1 + 2 * 4] = -xsin;
	x.m[2 + 1 * 4] = xsin;x.m[2 + 2 * 4] = xcos;

	y.m[0 + 0 * 4] = ycos;y.m[0 + 2 * 4] = -ysin;
	y.m[2 + 0 * 4] = ysin;y.m[2 + 2 * 4] = ycos;

	z.m[0 + 0 * 4] = zcos;z.m[0 + 1 * 4] = -zsin;
	z.m[1 + 0 * 4] = zsin;z.m[1 + 1 * 4] = zcos;

	return z * y * x;
}

mat3 mat3::Scale(const vec3& v) {
	mat3 tmp(1);

	tmp.m[0 + 0 * 4] = v.x();
	tmp.m[1 + 1 * 4] = v.y();
	tmp.m[2 + 2 * 4] = v.z();

	return tmp;
}

mat3 mat3::operator*(const mat3& r) {
	mat3 tmp;
	__m128 col[3];

	col[0] = _mm_set_ps(0, r.m[0 + 2 * 4], r.m[0 + 1 * 4], r.m[0 + 0 * 4]);
	col[1] = _mm_set_ps(0, r.m[1 + 2 * 4], r.m[1 + 1 * 4], r.m[1 + 0 * 4]);
	col[2] = _mm_set_ps(0, r.m[2 + 2 * 4], r.m[2 + 1 * 4], r.m[2 + 0 * 4]);

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			__m128 res = _mm_mul_ps(row[x], col[y]);
			tmp.m[x + y * 4] = res.m128_f32[0] + res.m128_f32[1] + res.m128_f32[2];
		}
	}

	return tmp;
}

vec3 mat3::operator*(const vec3& v) {
	__m128 vec[3];

	vec[0] = _mm_set_ps(0, v.x(), v.x(), v.x());
	vec[1] = _mm_set_ps(0, v.y(), v.y(), v.y());
	vec[2] = _mm_set_ps(0, v.z(), v.z(), v.z());

	__m128 res = _mm_mul_ps(vec[0], row[0]);

	for (int i = 1; i < 3; i++)
		res = _mm_add_ps(res, _mm_mul_ps(vec[i], row[i]));

	return vec3(res);
}