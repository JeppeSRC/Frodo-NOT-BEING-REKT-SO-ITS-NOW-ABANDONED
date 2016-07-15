#include "usercamera.h"
#include <core/input.h>
#include <core/window.h>

#define SENSE 8.0f

static vec3 left(-1, 0, 0);
static vec3 right(1, 0, 0);
static vec3 forward(0, 0, 1);
static vec3 back(0, 0, -1);

void UserCamera::Update(float delta) {

	vec3 move(0, 0, 0);

	if (Input::IsKeyDown('W')) {
		move += forward;
	}
	else if (Input::IsKeyDown('S'))
		move += back;

	if (Input::IsKeyDown('A'))
		move += left;
	else if (Input::IsKeyDown('D'))
		move += right;

	if (Input::IsKeyDownOnce('Q')) {
		FD_DEBUG("B");
		if (Input::mouseCaptured)
			Input::ReleaseMouse();
		else
			Input::CaptureMouse();
	}

	if (Input::mouseCaptured) {

		Window& window = *D3DContext::GetWindow();

		float yRot = ((float)Input::GetMouseX() - (window.GetWidth() >> 1)) * delta * SENSE;
		float xRot = ((float)Input::GetMouseY() - (window.GetHeight() >> 1)) * delta * SENSE;

		FD_DEBUG("%u", Input::GetMouseY());

		rotation += vec3(-xRot, yRot, 0);

	}

		position += move.RotateY(rotation.GetY()) * delta;

	UpdateViewMatrix();
}