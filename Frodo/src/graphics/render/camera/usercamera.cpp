#include "usercamera.h"
#include <core/input.h>
#include <core/window.h>

#define SENSE 8.0f

static vec3 left(-1, 0, 0);
static vec3 right(1, 0, 0);
static vec3 forward(0, 0, 1);
static vec3 back(0, 0, -1);

void UserCamera::Update(float delta) {

	vec3 move(dir);

	if (Input::mouseCaptured) {

		Window& window = *D3DContext::GetWindow();

		float yRot = (x - (window.GetWidth() >> 1)) * delta * SENSE;
		float xRot = (y - (window.GetHeight() >> 1)) * delta * SENSE;

		rotation += vec3(-xRot, yRot, 0);

	}

		position += move.RotateY(rotation.y) * delta;

	UpdateViewMatrix();
}

bool UserCamera::OnKeyboardActionKeyPressed(unsigned int key) {

	if (key == 'W') {
		dir += forward;
	}
	else if (key == 'S') {
		dir += back;
	}

	if (key == 'A') {
		dir += left;
	}
	else if (key == 'D') {
		dir += right;
	}

	if (key == 'Q')
		if (Input::mouseCaptured)
			Input::ReleaseMouse();
		else
			Input::CaptureMouse();

	return false;
}

bool UserCamera::OnKeyboardActionKeyReleased(unsigned int key) {

	if (key == 'W') {
		dir -= forward;
	}
	else if (key == 'S') {
		dir -= back;
	}

	if (key == 'A') {
		dir -= left;
	}
	else if (key == 'D') {
		dir -= right;
	}

	return false;
}

bool UserCamera::OnMouseActionMove(ivec2 position) {
	x = (float)position.x;
	y = (float)position.y;

	return false;
}