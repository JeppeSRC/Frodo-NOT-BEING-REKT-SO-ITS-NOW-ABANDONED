#include "math.h"
#include <memory>

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

	x.m[1 + 1 * 4] = xcos;x.m[1 + 2 * 4] = -xsin;
	x.m[2 + 1 * 4] = xsin;x.m[2 + 2 * 4] = xcos;

	y.m[0 + 0 * 4] = ycos;y.m[0 + 2 * 4] = -ysin;
	y.m[2 + 0 * 4] = ysin;y.m[2 + 2 * 4] = ycos;

	z.m[0 + 0 * 4] = zcos;z.m[0 + 1 * 4] = -zsin;
	z.m[1 + 0 * 4] = zsin;z.m[1 + 1 * 4] = zcos;
	
	return x * y * z;
}

mat4 mat4::Scale(const vec3& v) {
	mat4 tmp(1);

	tmp.m[0 + 0 * 4] = v.x;
	tmp.m[1 + 1 * 4] = v.y;
	tmp.m[2 + 2 * 4] = v.z;

	return tmp;
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

	m[0 + 0 * 4] = 2.0f / (right - left);
	m[1 + 1 * 4] = 2.0f / (top - bottom);
	m[2 + 2 * 4] = -2.0f / (zFar - zNear);

	m[0 + 3 * 4] = -((right + left) / (right - left));
	m[1 + 3 * 4] = -((top + bottom) / (top - bottom));
	m[2 + 3 * 4] = -((zFar + zNear) / (zNear - zFar));
	m[3 + 3 * 4] = 1;
	
	return r;
}

mat4 mat4::operator*(const mat4& r) {
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

vec4 mat4::operator*(const vec4& v) {
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

vec3 mat4::operator*(const vec3& v) {
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