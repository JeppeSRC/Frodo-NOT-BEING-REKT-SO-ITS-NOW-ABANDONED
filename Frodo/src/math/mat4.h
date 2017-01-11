#pragma once
#include "mathcommon.h"

class FDAPI mat4 {
private:
	friend class mat3;
	friend class vec3;
	friend class vec4;
private:
	union {
		float32 m[16];
	};

	void LoadRows(__m128* xmm) const;
	void LoadColumns(__m128* xmm) const;

public:
	mat4();
	mat4(float32 diagonal);

	inline static mat4 Identity() { return mat4(1); }

	static mat4 Translate(const vec3& v);
	static mat4 Rotate(const vec3& v);
	static mat4 Scale(const vec3& v);

	static mat4 Perspective(float32 fov, float32 aspect, float32 zNear, float32 zFar);
	static mat4 Orthographic(float32 left, float32 right, float32 top, float32 bottom, float32 zNear, float32 zFar);

	mat4 operator*(const mat4& m);
	vec4 operator*(const vec4& v);
	vec3 operator*(const vec3& v);

	__forceinline const float32* GetData() const { return m; }
};
