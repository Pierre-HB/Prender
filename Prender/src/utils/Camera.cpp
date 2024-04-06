#include "Camera.h"

mat4 Camera::getViewMatrix() {
	return inverse(camera);
}

mat4 Camera::getProjectionMatrix() {
	return projection;
}

mat4 lookat(const vec3& target, const vec3& position, const vec3& up) {
    vec3 forward = normalize(target - position);
    vec3 z = -forward; //il opengl the forward vector is in the direction -z
    vec3 x = normalize(up) ^ z;
    vec3 y = z ^ x;
    /*
    * Lookat =
    *
    * X.x X.y X.z P.x
    * Y.x Y.y Y.z P.y
    * Z.x Z.y Z.z P.z
    * 0   0   0    1
    *
    */

    return mat4(x.x, x.y, x.z, position.x, y.x, y.y, y.z, position.y, z.x, z.y, z.z, position.z, 0, 0, 0, 1);
}

mat4 perspectiveProjection(float fov, float aspect_ratio, float z_near, float z_far) {
    //aspect_ratio = with/height
    /*
    * 
    * z_near/r = 1/tan(fov/2)
    * 
    * z_near/t = z_near/(r/aspect_ratio) = aspect_ratio*z_near/r = aspect_ratio/tan(fov/2)
    * 
    * matrix=
    * 1/tan(fov/2) 0 0 0
    * 0 aspect_ratio/tan(fov/2) 0 0
    * 0 0 -(z_far+z_near)/(z_far-z_near)  -2*z_far*z_near/(z_far-z_near)
    * 0 0 -1 0
    * 
    */

    float inv_tan = 1 / tan(fov / 2);
    return mat4(inv_tan, 0, 0, 0,
        0, aspect_ratio * inv_tan, 0, 0,
        0, 0, -(z_far + z_near) / (z_far - z_near), -2 * z_far * z_near / (z_far - z_near),
        0, 0, -1, 0);    
    return mat4(inv_tan, 0, 0, 0,
            0, aspect_ratio * inv_tan, 0, 0,
            0, 0, -(z_far + z_near) / (z_far - z_near), 2 * z_far * z_near / (z_far - z_near),
            0, 0, 1, 0);
}

#ifdef IMGUI
void* Camera::getAttribute() const {
    return new imGuiCameraAttr(camera);
}

void Camera::updateAttribute(void* attr) const {
    static_cast<imGuiCameraAttr*>(attr)->camera.updateAttr(camera);
}

void Camera::setAttribute(void* attr) {
    camera = static_cast<imGuiCameraAttr*>(attr)->camera.getTansformation();
}

void Camera::imGuiPrintAttribute(void* attr) const {
    static_cast<imGuiCameraAttr*>(attr)->camera.imGuiPrintAttribute();
}
#endif