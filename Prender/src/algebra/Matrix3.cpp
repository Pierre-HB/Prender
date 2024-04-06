#include "Matrix3.h"

#ifdef CONSOLE
std::ostream& operator<<(std::ostream& o, const mat3& m) {
	o << "[" << m.c[0] << ", " << m.c[1] << ", " << m.c[2] << " | " << m.c[3] << ", " << m.c[4] << ", " << m.c[5] << " | " << m.c[6] << ", " << m.c[7] << ", " << m.c[8] << "]";
	return o;
}
#endif

mat3* mat3::operator+=(const mat3& m) {
	for (int i = 0; i < 9; i++)
		c[i] += m.c[i];
	return this;
}

mat3* mat3::operator-=(const mat3& m) {
	for (int i = 0; i < 9; i++)
		c[i] -= m.c[i];
	return this;
}

mat3* mat3::operator*=(const mat3& m)
{
	float tmp[9]{ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	/*
	| tmp[0]  tmp[1]  tmp[2]
	| tmp[3]  tmp[4]  tmp[5]
	| tmp[6]  tmp[7]  tmp[8]
	*/
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				tmp[3 * i + j] += c[3 * i + k] * m.c[3 * k + j];
	for (int i = 0; i < 9; i++)
		c[i] = tmp[i];
	return this;
}

mat3* mat3::operator*=(float s) {
	for (int i = 0; i < 9; i++)
		c[i] *= s;
	return this;
}

mat3* mat3::operator/=(float s) {
	for (int i = 0; i < 9; i++)
		c[i] /= s;
	return this;
}

mat3 operator+(const mat3& a, const mat3& b) {
	mat3 tmp = mat3();
	for (int i = 0; i < 9; i++)
		tmp.c[i] = a.c[i] + b.c[i];
	return tmp;
}

mat3 operator-(const mat3& a, const mat3& b) {
	mat3 tmp = mat3();
	for (int i = 0; i < 9; i++)
		tmp.c[i] = a.c[i] - b.c[i];
	return tmp;
}

mat3 operator*(const mat3& a, const mat3& b) {
	mat3 tmp = mat3();
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				tmp.c[3 * i + j] += a.c[3 * i + k] * b.c[3 * k + j];
	return tmp;
}

vec3 operator*(const mat3& m, const vec3& v) {
	vec3 tmp = vec3();

	tmp.x = m.c[0] * v.x + m.c[1] * v.y + m.c[2] * v.z;
	tmp.y = m.c[3] * v.x + m.c[4] * v.y + m.c[5] * v.z;
	tmp.z = m.c[6] * v.x + m.c[7] * v.y + m.c[8] * v.z;

	return tmp;
}

mat3 operator*(const mat3& m, float s) {
	mat3 tmp = mat3();
	for (int i = 0; i < 9; i++)
		tmp.c[i] = m.c[i] * s;
	return tmp;
}

mat3 operator*(float s, const mat3& m) {
	return m * s;
}

mat3 operator/(const mat3& m, float s) {
	mat3 tmp = mat3();
	for (int i = 0; i < 9; i++)
		tmp.c[i] = m.c[i] / s;
	return tmp;
}

mat3 transpose(const mat3& m) {
	mat3 tmp = mat3();
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			tmp.c[3 * i + j] = m.c[3 * j + i];
	return tmp;
}

mat3 inverse(const mat3& m) {


	double det = m.c[0] * (m.c[4] * m.c[8] - m.c[7] * m.c[5]) -
		m.c[1] * (m.c[3] * m.c[8] - m.c[5] * m.c[6]) +
		m.c[2] * (m.c[3] * m.c[7] - m.c[4] * m.c[6]);

	float invdet = 1 / det;

	mat3 tmp = mat3(); // inverse of matrix m
	tmp.c[0] = (m.c[4] * m.c[8] - m.c[7] * m.c[5]) * invdet;
	tmp.c[1] = (m.c[2] * m.c[7] - m.c[1] * m.c[8]) * invdet;
	tmp.c[2] = (m.c[1] * m.c[5] - m.c[2] * m.c[4]) * invdet;
	tmp.c[3] = (m.c[5] * m.c[6] - m.c[3] * m.c[8]) * invdet;
	tmp.c[4] = (m.c[0] * m.c[8] - m.c[2] * m.c[6]) * invdet;
	tmp.c[5] = (m.c[3] * m.c[2] - m.c[0] * m.c[5]) * invdet;
	tmp.c[6] = (m.c[3] * m.c[7] - m.c[6] * m.c[4]) * invdet;
	tmp.c[7] = (m.c[6] * m.c[1] - m.c[0] * m.c[7]) * invdet;
	tmp.c[8] = (m.c[0] * m.c[4] - m.c[3] * m.c[1]) * invdet;

	return tmp;
}

