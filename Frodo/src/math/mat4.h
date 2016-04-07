#pragma once
#include "mathcommon.h"

class FDAPI mat4 {
private:
	friend class mat3;
	friend class vec3;
	friend class vec4;
private:
	union {
		__m128 row[4];
		float m[16];
	};

public:
	mat4();
	mat4(float diagonal);

	inline static mat4 Identity() { return mat4(1); }

	static mat4 Translate(const vec3& v);
	static mat4 Rotate(const vec3& v);
	static mat4 Scale(const vec3& v);

	mat4 operator*(const mat4& m);
	vec4 operator*(const vec4& v);
	vec3 operator*(const vec3& v);

	__forceinline const float* GetData() const { return m; }
};
