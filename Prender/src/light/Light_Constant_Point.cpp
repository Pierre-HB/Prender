#include "Light_Constant_Point.h"

Light_Constant_Point::Light_Constant_Point(const vec3& lightColor, const vec3& position) : Light(lightColor, 0, position, 0, vec3(), 0) {

}

Light_Constant_Point::~Light_Constant_Point() {

}