#include "usercamera.h"
#include <core/input.h>
#include <core/window.h>
#include <audio/audiomanager.h>

namespace FD {

#define SENSE 0.25f

#define SPEED 2.0f

static vec3 left(-SPEED, 0, 0);
static vec3 right(SPEED, 0, 0);
static vec3 forward(0, 0, SPEED);
static vec3 back(0, 0, -SPEED);

void UserCamera::Update(float32 delta) {
	velocity = dir;
	
	position += velocity.RotateY(rotation.y) * delta;

	UpdateViewMatrix();
	AudioManager::UpdateListener(this);
}

bool UserCamera::OnKeyboardActionKeyPressed(FD_KEY key) {

	if (Input::IsMouseAcquired()) {
		if (key == FD_KEY_W) {
			dir += forward;
		} else if (key == FD_KEY_S) {
			dir += back;
		}

		if (key == FD_KEY_A) {
			dir += left;
		} else if (key == FD_KEY_D) {
			dir += right;
		}
	}
	if (key == FD_KEY_Q)
		Input::ToggleMouseAcquisition();

	if (key == FD_KEY_TAB) {
		D3DContext::SetFullscreen(true);
	} else if (key == FD_KEY_ESCAPE) {
		D3DContext::SetFullscreen(false);
	}


	return false;
}

bool UserCamera::OnKeyboardActionKeyReleased(FD_KEY key) {

	if (Input::IsMouseAcquired()) {
		if (key == FD_KEY_W) {
			dir -= forward;
		} else if (key == FD_KEY_S) {
			dir -= back;
		}

		if (key == FD_KEY_A) {
			dir -= left;
		} else if (key == FD_KEY_D) {
			dir -= right;
		}

	}

	return false;
}

bool UserCamera::OnMouseActionMoveRelative(ivec2 position) {
	rotation.x -= (float32)position.y * SENSE;
	rotation.y += (float32)position.x * SENSE;

	return false;
}

bool UserCamera::OnWindowStateChanged(FD_EVENT_ACTION state) {
	if (state == FD_FOCUS_LOST) {
		dir *= 0.0f;
	}

	return false;
}

}