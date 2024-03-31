#pragma once

#include "../../main.h"
#include "../algebra/Algebra.h"
class Camera
{
protected:
	//! Camera transform : the world space transformation of the camera. To transform an object in world space in view space, juste multiply my inverse(camera)
	mat4 camera;
	//! Projection transform : The transformation from the view space to the clip space
	mat4 projection;

public:
	//! constructor from a camera transform and a projection transform
	Camera(mat4 camera, mat4 projection) : camera(camera), projection(projection){}

	~Camera() {}

	//! return the view matrix (view = inverse(camera))
	mat4 getViewMatrix();

	//! return the projection matrix
	mat4 getProjectionMatrix();
};

//! return the lookat camera matrix
mat4 lookat(const vec3& target, const vec3& position, const vec3& up);

//! return the perspective projection with an infinit z_far
mat4 perspectiveProjection(float fov, float aspect_ratio, float z_near);

//! return the perspective projection
mat4 perspectiveProjection(float fov, float aspect_ratio, float z_near, float z_far);

//! return the orthogonal projection
mat4 orthogonalProjection(float apect_ratio, float z_near, float z_far);
