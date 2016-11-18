#pragma once

#include "camera.h"
#include <core/event/eventlistener.h>
#include <core/event/eventdispatcher.h>


class FDAPI UserCamera : public Camera, public EventListener {
private:
	vec3 dir;
	float x,  y;

public:
	UserCamera(const vec3& position, const vec3& rotation) : Camera(position, rotation) {  }
	~UserCamera() { }

	virtual void Update(float delta);
	bool OnEvent(const Event* event) override { return false; }
	bool OnKeyboardActionKeyPressed(unsigned int key) override;
	bool OnKeyboardActionKeyReleased(unsigned int key) override;
	bool OnMouseActionMove(ivec2 position) override;
};
