#pragma once

#include "Camera.h"

#ifdef IMGUI
struct imGuiPerspectiveCameraAttr {
	float aspect_ratio;
	float fov;
	float z_near;
	float z_far;
	void* parentAttr;



	imGuiPerspectiveCameraAttr(float aspect_ratio, float fov, float z_near, float z_far, void* parentAttr) :aspect_ratio(aspect_ratio), fov(fov), z_near(z_near), z_far(z_far), parentAttr(parentAttr) {}
};

class PerspectiveCamera : public Camera, public ImGuiPrintable
#else
class PerspectiveCamera : public Camera
#endif
{
private:

	float aspect_ratio;
	float fov;
	float z_near;
	float z_far;

public:

	PerspectiveCamera(float aspect_ratio, float fov, float z_near, float z_far, const mat4& camera = mat4::identity());

	~PerspectiveCamera();

	void moveWorld(const mat4& transormation);

	void moveView(const mat4& transormation);

#ifdef IMGUI
	virtual void* getAttribute() const;

	virtual void updateAttribute(void* attr) const;

	virtual void setAttribute(void* attr);

	virtual void imGuiPrintAttribute(void* attr) const;
#endif
};