#pragma once

#include "Camera.h"
#include "../../main.h"

#ifdef IMGUI
//! data for a Perpective Camera UI
struct imGuiPerspectiveCameraAttr {
	float aspect_ratio;
	float fov;
	float z_near;
	float z_far;
	void* parentAttr;

	//! Constructor
	imGuiPerspectiveCameraAttr(float aspect_ratio, float fov, float z_near, float z_far, void* parentAttr) :aspect_ratio(aspect_ratio), fov(fov), z_near(z_near), z_far(z_far), parentAttr(parentAttr) {
#ifdef DEBUG
		debug::NB_ATTR++;
#endif
	}

	~imGuiPerspectiveCameraAttr() {
		//delete parentAttr;
#ifdef DEBUG
		debug::NB_ATTR--;
#endif
	}
};

//! a perspective camera
class PerspectiveCamera : public Camera, public ImGuiPrintable
#else
//! a perspective camera
class PerspectiveCamera : public Camera
#endif
{
private:

	float aspect_ratio;
	float fov;
	float z_near;
	float z_far;

public:

	//! Constructor of a Perspective Camera
	PerspectiveCamera(float aspect_ratio, float fov, float z_near, float z_far, const mat4& camera = mat4::identity());

	~PerspectiveCamera();

	//! moove the camera in the world space
	void moveWorld(const mat4& transormation);

	//! moove the camera in the view space
	void moveView(const mat4& transormation);

#ifdef IMGUI
	//! \copydoc ImGuiPrintable::getAttribute()
	virtual void* getAttribute() const;

	//! \copydoc ImGuiPrintable::updateAttribute()
	virtual void updateAttribute(void* attr) const;

	//! \copydoc ImGuiPrintable::setAttribute()
	virtual void setAttribute(void* attr);

	//! \copydoc ImGuiPrintable::imGuiPrintAttribute()
	virtual void imGuiPrintAttribute(void* attr) const;

	//! \copydoc ImGuiPrintable::deleteAttribute()
	virtual void deleteAttribute(void* attr) const;
#endif
};