#pragma once
#include "mathcommon.h"

class FDAPI mat4 {
private:
	friend class mat3;
	friend class vec3;
	friend class vec4;
private:
	union {
		float m[16];
	};

	void LoadRows(__m128* xmm) const;
	void LoadColumns(__m128* xmm) const;

public:
	mat4();
	mat4(float diagonal);

	inline static mat4 Identity() { return mat4(1); }

	static mat4 Translate(const vec3& v);
	static mat4 Rotate(const vec3& v);
	static mat4 Scale(const vec3& v);

	static mat4 Inverse(mat4 m);

	static mat4 Perspective(float fov, float aspect, float zNear, float zFar);
	static mat4 Orthographic(float left, float right, float top, float bottom, float zNear, float zFar);

	mat4 operator*(const mat4& m);
	vec4 operator*(const vec4& v);
	vec3 operator*(const vec3& v);

	__forceinline const float* GetData() const { return m; }
};
