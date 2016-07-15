#pragma once

#include "camera.h"


class FDAPI UserCamera : public Camera {
private:
	

public:
	UserCamera(const vec3& position, const vec3& rotation) : Camera(position, rotation) { }


	virtual void Update(float delta);
};
