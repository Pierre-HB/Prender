#pragma once

#include "../algebra/Algebra.h"
#include "../../main.h"

#ifdef IMGUI
//! Data for the Camera UI
struct imGuiCameraAttr {
	ImGuiTransformationAttr camera;

	//! constructor
	imGuiCameraAttr(const mat4& camera) : camera(ImGuiTransformationAttr(camera, "camera")) {
#ifdef DEBUG
		debug::NB_ATTR++;
#endif
	}

	~imGuiCameraAttr() {
#ifdef DEBUG
		debug::NB_ATTR--;
#endif
	}
};

//! a virtual class for cameras
class Camera : public ImGuiPrintable
#else
//! a virtual class for cameras
class Camera
#endif
{
protected:
	//! Camera transform : the world space transformation of the camera. To transform an object in world space in view space, juste multiply my inverse(camera)
	mat4 camera;
	//! Projection transform : The transformation from the view space to the clip space
	mat4 projection;

public:
	//! constructor from a camera transform and a projection transform
	Camera(mat4 camera, mat4 projection) : camera(camera), projection(projection){
#ifdef DEBUG
		debug::NB_INSTANCES++;
#endif
	}

	~Camera() {
#ifdef DEBUG
		debug::NB_INSTANCES--;
#endif
	}

	//! return the view matrix (view = inverse(camera))
	mat4 getViewMatrix();

	//! return the projection matrix
	mat4 getProjectionMatrix();

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

//! return the lookat camera matrix
mat4 lookat(const vec3& target, const vec3& position, const vec3& up);

//! return the perspective projection with an infinit z_far
mat4 perspectiveProjection(float fov, float aspect_ratio, float z_near);

//! return the perspective projection
mat4 perspectiveProjection(float fov, float aspect_ratio, float z_near, float z_far);

//! return the orthogonal projection
mat4 orthogonalProjection(float apect_ratio, float z_near, float z_far);
