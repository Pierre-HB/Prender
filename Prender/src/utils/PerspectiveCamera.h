#pragma once

#include "Camera.h"

class PerspectiveCamera : public Camera
{
private:

	float aspect_ratio;
	float fov;
	float z_near;
	float z_far;

public:

	PerspectiveCamera(float aspect_ratio, float fov, float z_near, float z_far);

	~PerspectiveCamera();

	void moveWorld(const mat4& transormation);

	void moveView(const mat4& transormation);


};