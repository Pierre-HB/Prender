#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(float aspect_ratio, float fov, float z_near, float z_far) : aspect_ratio(aspect_ratio), fov(fov), z_near(z_near), z_far(z_far), Camera(mat4::identity(), perspectiveProjection(fov, aspect_ratio, z_near, z_far)) {

}

PerspectiveCamera::~PerspectiveCamera() {

}

void PerspectiveCamera::moveWorld(const mat4& transormation) {
	camera *= transormation;
}

void PerspectiveCamera::moveView(const mat4& transormation) {
	camera = transormation * camera;
}