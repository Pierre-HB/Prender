#include "Matrix4.h"
#include <iostream>

std::ostream& operator<<(std::ostream& o, const mat4& m) {
	o << "[" << m.c[0] << ", " << m.c[1] << ", " << m.c[2] << ", " << m.c[3] << " | " << m.c[4] << ", " << m.c[5] << ", " << m.c[6] << ", " << m.c[7] << " | " << m.c[8] << ", " << m.c[9] << ", " << m.c[10] << ", " << m.c[11] << " | " << m.c[12] << ", " << m.c[13] << ", " << m.c[14] << ", " << m.c[15] << "]";
	return o;
}

mat4* mat4::operator+=(const mat4& m) {
	for (int i = 0; i < 16; i++)
		c[i] += m.c[i];
	return this;
}

mat4* mat4::operator-=(const mat4& m) {
	for (int i = 0; i < 16; i++)
		c[i] -= m.c[i];
	return this;
}

mat4* mat4::operator*=(const mat4& m)
{
	float tmp[16]{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	/*
	| tmp[0 ]  tmp[1 ]  tmp[2 ]  tmp[3 ]
	| tmp[4 ]  tmp[5 ]  tmp[6 ]  tmp[7 ]
	| tmp[8 ]  tmp[9 ]  tmp[10]  tmp[11]
	| tmp[12]  tmp[13]  tmp[14]  tmp[15]
	*/
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				tmp[4 * i + j] += c[4 * i + k] * m.c[4 * k + j];
	for (int i = 0; i < 16; i++)
		c[i] = tmp[i];
	return this;
}

mat4* mat4::operator*=(float s) {
	for (int i = 0; i < 16; i++)
		c[i] *= s;
	return this;
}

mat4* mat4::operator/=(float s) {
	for (int i = 0; i < 16; i++)
		c[i] /= s;
	return this;
}

mat4 operator+(const mat4& a, const mat4& b) {
	mat4 tmp = mat4();
	for (int i = 0; i < 16; i++)
		tmp.c[i] = a.c[i] + b.c[i];
	return tmp;
}

mat4 operator-(const mat4& a, const mat4& b) {
	mat4 tmp = mat4();
	for (int i = 0; i < 16; i++)
		tmp.c[i] = a.c[i] - b.c[i];
	return tmp;
}

mat4 operator*(const mat4& a, const mat4& b) {
	mat4 tmp = mat4();
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				tmp.c[4 * i + j] += a.c[4 * i + k] * b.c[4 * k + j];
	return tmp;
}

vec4 operator*(const mat4& m, const vec4& v) {
	vec4 tmp = vec4();

	tmp.x = m.c[0] * v.x + m.c[1] * v.y + m.c[2] * v.z + m.c[3] * v.w;
	tmp.y = m.c[4] * v.x + m.c[5] * v.y + m.c[6] * v.z + m.c[7] * v.w;
	tmp.z = m.c[8] * v.x + m.c[9] * v.y + m.c[10] * v.z + m.c[11] * v.w;
	tmp.w = m.c[12] * v.x + m.c[13] * v.y + m.c[14] * v.z + m.c[15] * v.w;

	return tmp;
}

vec3 operator*(const mat4& m, const vec3& v) {
	vec3 tmp = vec3();

	tmp.x = m.c[0] * v.x + m.c[1] * v.y + m.c[2] * v.z + m.c[3];
	tmp.y = m.c[4] * v.x + m.c[5] * v.y + m.c[6] * v.z + m.c[7];
	tmp.z = m.c[8] * v.x + m.c[9] * v.y + m.c[10] * v.z + m.c[11];
	float w = m.c[12] * v.x + m.c[13] * v.y + m.c[14] * v.z + m.c[15];

	return tmp / w;
}

mat4 operator*(const mat4& m, float s) {
	mat4 tmp = mat4();
	for (int i = 0; i < 16; i++)
		tmp.c[i] = m.c[i] * s;
	return tmp;
}

mat4 operator*(float s, const mat4& m) {
	return m * s;
}

mat4 operator/(const mat4& m, float s) {
	mat4 tmp = mat4();
	for (int i = 0; i < 16; i++)
		tmp.c[i] = m.c[i] / s;
	return tmp;
}

mat4 transpose(const mat4& m) {
	mat4 tmp = mat4();
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			tmp.c[4 * i + j] = m.c[4 * j + i];
	return tmp;
}

mat4 inverse(const mat4& m) {

    mat4 tmp = mat4();
    tmp.c[0] = m.c[5] * m.c[10] * m.c[15] -
        m.c[5] * m.c[11] * m.c[14] -
        m.c[9] * m.c[6] * m.c[15] +
        m.c[9] * m.c[7] * m.c[14] +
        m.c[13] * m.c[6] * m.c[11] -
        m.c[13] * m.c[7] * m.c[10];

    tmp.c[4] = -m.c[4] * m.c[10] * m.c[15] +
        m.c[4] * m.c[11] * m.c[14] +
        m.c[8] * m.c[6] * m.c[15] -
        m.c[8] * m.c[7] * m.c[14] -
        m.c[12] * m.c[6] * m.c[11] +
        m.c[12] * m.c[7] * m.c[10];

    tmp.c[8] = m.c[4] * m.c[9] * m.c[15] -
        m.c[4] * m.c[11] * m.c[13] -
        m.c[8] * m.c[5] * m.c[15] +
        m.c[8] * m.c[7] * m.c[13] +
        m.c[12] * m.c[5] * m.c[11] -
        m.c[12] * m.c[7] * m.c[9];

    tmp.c[12] = -m.c[4] * m.c[9] * m.c[14] +
        m.c[4] * m.c[10] * m.c[13] +
        m.c[8] * m.c[5] * m.c[14] -
        m.c[8] * m.c[6] * m.c[13] -
        m.c[12] * m.c[5] * m.c[10] +
        m.c[12] * m.c[6] * m.c[9];

    tmp.c[1] = -m.c[1] * m.c[10] * m.c[15] +
        m.c[1] * m.c[11] * m.c[14] +
        m.c[9] * m.c[2] * m.c[15] -
        m.c[9] * m.c[3] * m.c[14] -
        m.c[13] * m.c[2] * m.c[11] +
        m.c[13] * m.c[3] * m.c[10];

    tmp.c[5] = m.c[0] * m.c[10] * m.c[15] -
        m.c[0] * m.c[11] * m.c[14] -
        m.c[8] * m.c[2] * m.c[15] +
        m.c[8] * m.c[3] * m.c[14] +
        m.c[12] * m.c[2] * m.c[11] -
        m.c[12] * m.c[3] * m.c[10];

    tmp.c[9] = -m.c[0] * m.c[9] * m.c[15] +
        m.c[0] * m.c[11] * m.c[13] +
        m.c[8] * m.c[1] * m.c[15] -
        m.c[8] * m.c[3] * m.c[13] -
        m.c[12] * m.c[1] * m.c[11] +
        m.c[12] * m.c[3] * m.c[9];

    tmp.c[13] = m.c[0] * m.c[9] * m.c[14] -
        m.c[0] * m.c[10] * m.c[13] -
        m.c[8] * m.c[1] * m.c[14] +
        m.c[8] * m.c[2] * m.c[13] +
        m.c[12] * m.c[1] * m.c[10] -
        m.c[12] * m.c[2] * m.c[9];

    tmp.c[2] = m.c[1] * m.c[6] * m.c[15] -
        m.c[1] * m.c[7] * m.c[14] -
        m.c[5] * m.c[2] * m.c[15] +
        m.c[5] * m.c[3] * m.c[14] +
        m.c[13] * m.c[2] * m.c[7] -
        m.c[13] * m.c[3] * m.c[6];

    tmp.c[6] = -m.c[0] * m.c[6] * m.c[15] +
        m.c[0] * m.c[7] * m.c[14] +
        m.c[4] * m.c[2] * m.c[15] -
        m.c[4] * m.c[3] * m.c[14] -
        m.c[12] * m.c[2] * m.c[7] +
        m.c[12] * m.c[3] * m.c[6];

    tmp.c[10] = m.c[0] * m.c[5] * m.c[15] -
        m.c[0] * m.c[7] * m.c[13] -
        m.c[4] * m.c[1] * m.c[15] +
        m.c[4] * m.c[3] * m.c[13] +
        m.c[12] * m.c[1] * m.c[7] -
        m.c[12] * m.c[3] * m.c[5];

    tmp.c[14] = -m.c[0] * m.c[5] * m.c[14] +
        m.c[0] * m.c[6] * m.c[13] +
        m.c[4] * m.c[1] * m.c[14] -
        m.c[4] * m.c[2] * m.c[13] -
        m.c[12] * m.c[1] * m.c[6] +
        m.c[12] * m.c[2] * m.c[5];

    tmp.c[3] = -m.c[1] * m.c[6] * m.c[11] +
        m.c[1] * m.c[7] * m.c[10] +
        m.c[5] * m.c[2] * m.c[11] -
        m.c[5] * m.c[3] * m.c[10] -
        m.c[9] * m.c[2] * m.c[7] +
        m.c[9] * m.c[3] * m.c[6];

    tmp.c[7] = m.c[0] * m.c[6] * m.c[11] -
        m.c[0] * m.c[7] * m.c[10] -
        m.c[4] * m.c[2] * m.c[11] +
        m.c[4] * m.c[3] * m.c[10] +
        m.c[8] * m.c[2] * m.c[7] -
        m.c[8] * m.c[3] * m.c[6];

    tmp.c[11] = -m.c[0] * m.c[5] * m.c[11] +
        m.c[0] * m.c[7] * m.c[9] +
        m.c[4] * m.c[1] * m.c[11] -
        m.c[4] * m.c[3] * m.c[9] -
        m.c[8] * m.c[1] * m.c[7] +
        m.c[8] * m.c[3] * m.c[5];

    tmp.c[15] = m.c[0] * m.c[5] * m.c[10] -
        m.c[0] * m.c[6] * m.c[9] -
        m.c[4] * m.c[1] * m.c[10] +
        m.c[4] * m.c[2] * m.c[9] +
        m.c[8] * m.c[1] * m.c[6] -
        m.c[8] * m.c[2] * m.c[5];

    float invdet = 1/(m.c[0] * tmp.c[0] +
        m.c[1] * tmp.c[4] +
        m.c[2] * tmp.c[8] +
        m.c[3] * tmp.c[12]);

    return tmp * invdet;
}

mat4 rotationMatrixY(float a) {
    return mat4(cos(a),     0,   sin(a),     0,
                0,          1,  0,          0,
                -sin(a),    0,  cos(a),     0,
                0,          0,  0,          1);
}

mat4 translationMatrix(const vec3& v) {
    return mat4(1, 0, 0, v.x,
                0, 1, 0, v.y,
                0, 0, 1, v.z,
                0, 0, 0, 1);
}

mat4 normalTransformation(const mat4& m) {
    return transpose(inverse(m));
}