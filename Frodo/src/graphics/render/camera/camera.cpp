#include "camera.h"


void Camera::UpdateViewMatrix() {
	viewMatrix = mat4::Rotate(rotation) * mat4::Translate(-position);
}