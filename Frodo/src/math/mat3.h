#pragma once
#include "mathcommon.h"

namespace FD {

class FDAPI mat3 {
private:
	friend class vec3;
private:
	union {
		float32 m[9];
	};

	void LoadRows(__m128* xmm) const;
	void LoadColumns(__m128* xmm) const;

public:
	mat3();
	mat3(float32 diagonal);

	inline static mat3 Identity() { return mat3(1); }

	static mat3 Rotate(const vec3& v);
	static mat3 Scale(const vec3& v);

	mat3 operator*(const mat3& r);
	vec3 operator*(const vec3& v);

	__forceinline const float32* GetData() const { return m; }
};


}