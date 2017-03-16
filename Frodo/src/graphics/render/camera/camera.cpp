#include "camera.h"

namespace FD {

void Camera::UpdateViewMatrix() {
	viewMatrix = mat4::Rotate(rotation) * mat4::Translate(-position);
}

}