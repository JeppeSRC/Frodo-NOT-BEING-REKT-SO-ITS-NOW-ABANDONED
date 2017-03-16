#pragma once

#include "camera.h"
#include <core/event/eventlistener.h>
#include <core/event/eventdispatcher.h>

namespace FD {

class FDAPI UserCamera : public Camera, public EventListener{
private:
	vec3 dir;
	float32 x, y;

public:
	UserCamera(const vec3& position, const vec3& rotation) : Camera(position, rotation) {}
	~UserCamera() {}

	virtual void Update(float32 delta);
	bool OnEvent(const Event* event) override { return false; }
	bool OnKeyboardActionKeyPressed(uint32 key) override;
	bool OnKeyboardActionKeyReleased(uint32 key) override;
	bool OnMouseActionMove(ivec2 position) override;
};

}