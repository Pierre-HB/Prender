#include "PerspectiveCamera.h"


PerspectiveCamera::PerspectiveCamera(float aspect_ratio, float fov, float z_near, float z_far, const mat4& camera) : aspect_ratio(aspect_ratio), fov(fov), z_near(z_near), z_far(z_far), Camera(camera, perspectiveProjection(fov, aspect_ratio, z_near, z_far)) {
#ifdef IMGUI
	ImGuiManager::addObject(ImGuiObjectType::UTILS_PERSPECTIVE_CAMERA, this);
#endif
#ifdef DEBUG
	debug::NB_INSTANCES++;
#endif
}

PerspectiveCamera::~PerspectiveCamera() {
#ifdef IMGUI
	ImGuiManager::removeObject(ImGuiObjectType::UTILS_PERSPECTIVE_CAMERA, this);
#endif
#ifdef DEBUG
	debug::NB_INSTANCES--;
#endif
}

void PerspectiveCamera::moveWorld(const mat4& transormation) {
	camera *= transormation;
}

void PerspectiveCamera::moveView(const mat4& transormation) {
	camera = transormation * camera;
}

#ifdef IMGUI
void* PerspectiveCamera::getAttribute() const {
	return new imGuiPerspectiveCameraAttr(aspect_ratio, fov, z_near, z_far, Camera::getAttribute());
}

void PerspectiveCamera::updateAttribute(void* attr) const {
	Camera::updateAttribute(static_cast<imGuiPerspectiveCameraAttr*>(attr)->parentAttr);
	static_cast<imGuiPerspectiveCameraAttr*>(attr)->aspect_ratio = aspect_ratio;
	static_cast<imGuiPerspectiveCameraAttr*>(attr)->fov = fov;
	static_cast<imGuiPerspectiveCameraAttr*>(attr)->z_near = z_near;
	static_cast<imGuiPerspectiveCameraAttr*>(attr)->z_far = z_far;
}

void PerspectiveCamera::setAttribute(void* attr) {
	Camera::setAttribute(static_cast<imGuiPerspectiveCameraAttr*>(attr)->parentAttr);
	aspect_ratio = static_cast<imGuiPerspectiveCameraAttr*>(attr)->aspect_ratio;
	fov = static_cast<imGuiPerspectiveCameraAttr*>(attr)->fov;
	z_near = static_cast<imGuiPerspectiveCameraAttr*>(attr)->z_near;
	z_far = static_cast<imGuiPerspectiveCameraAttr*>(attr)->z_far;

	projection = perspectiveProjection(fov, aspect_ratio, z_near, z_far);
}

void PerspectiveCamera::imGuiPrintAttribute(void* attr) const {
	Camera::imGuiPrintAttribute(static_cast<imGuiPerspectiveCameraAttr*>(attr)->parentAttr);

	ImGui::SliderFloat("aspect ratio", &static_cast<imGuiPerspectiveCameraAttr*>(attr)->aspect_ratio, 1 / 3.0f, 3, NULL, ImGuiSliderFlags_Logarithmic);
	ImGui::SliderFloat("fov", &static_cast<imGuiPerspectiveCameraAttr*>(attr)->fov, 1e-3f, M_PI - 1e-3f);

	ImGui::DragFloat("z near", &static_cast<imGuiPerspectiveCameraAttr*>(attr)->z_near, 1, 1e-3f, static_cast<imGuiPerspectiveCameraAttr*>(attr)->z_far, NULL, ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("z far", &static_cast<imGuiPerspectiveCameraAttr*>(attr)->z_far, 1, static_cast<imGuiPerspectiveCameraAttr*>(attr)->z_near, 1e5f, NULL, ImGuiSliderFlags_Logarithmic);
}

void PerspectiveCamera::deleteAttribute(void* attr) const {
	Camera::deleteAttribute(static_cast<imGuiPerspectiveCameraAttr*>(attr)->parentAttr);
	delete static_cast<imGuiPerspectiveCameraAttr*>(attr);
}
#endif