#include "usercamera.h"
#include <core/input.h>
#include <core/window.h>

namespace FD {

#define SENSE 0.65f

#define SPEED 2.0f

static vec3 left(-SPEED, 0, 0);
static vec3 right(SPEED, 0, 0);
static vec3 forward(0, 0, SPEED);
static vec3 back(0, 0, -SPEED);

void UserCamera::Update(float32 delta) {

	vec3 move(dir);
	//TODO: mouse
	/*if (Input::mouseCaptured) {

		Window& window = *D3DContext::GetWindow();

		float32 yRot = (x - (window.GetWidth() >> 1)) * delta * SENSE;
		float32 xRot = (y - (window.GetHeight() >> 1)) * delta * SENSE;

		rotation += vec3(-xRot, yRot, 0);

	}*/

	position += move.RotateY(rotation.y) * delta;

	UpdateViewMatrix();
}

bool UserCamera::OnKeyboardActionKeyPressed(FD_KEY key) {

	if (key == 'W') {
		dir += forward;
	} else if (key == 'S') {
		dir += back;
	}

	if (key == 'A') {
		dir += left;
	} else if (key == 'D') {
		dir += right;
	}

	if (key == 'Q')
		Input::ToggleMouseAcquisition();

	return false;
}

bool UserCamera::OnKeyboardActionKeyReleased(FD_KEY key) {

	if (key == 'W') {
		dir -= forward;
	} else if (key == 'S') {
		dir -= back;
	}

	if (key == 'A') {
		dir -= left;
	} else if (key == 'D') {
		dir -= right;
	}

	return false;
}

bool UserCamera::OnMouseActionMoveRelative(ivec2 position) {
	rotation.x -= (float32)position.y * SENSE;
	rotation.y += (float32)position.x * SENSE;

	return false;
}

}