#include "Matrix2.h"

#ifdef CONSOLE
std::ostream& operator<<(std::ostream& o, const mat2& m) {
	o << "[" << m.c[0] << ", " << m.c[1] << " | " << m.c[2] << ", " << m.c[3] << "]";
	return o;
}
#endif

mat2* mat2::operator+=(const mat2& m) {
	for (int i = 0; i < 4; i++)
		c[i] += m.c[i];
	return this;
}

mat2* mat2::operator-=(const mat2& m) {
	for (int i = 0; i < 4; i++)
		c[i] -= m.c[i];
	return this;
}

mat2* mat2::operator*=(const mat2& m)
{
	float tmp[4]{ 0, 0, 0, 0 };
	/*
	| tmp[0]  tmp[1]
	| tmp[2]  tmp[3]
	*/
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
				tmp[2 * i + j] += c[2 * i + k] * m.c[2 * k + j];
	for (int i = 0; i < 4; i++)
		c[i] = tmp[i];
	return this;
}

mat2* mat2::operator*=(float s) {
	for (int i = 0; i < 4; i++)
		c[i] *= s;
	return this;
}

mat2* mat2::operator/=(float s) {
	for (int i = 0; i < 4; i++)
		c[i] /= s;
	return this;
}

mat2 operator+(const mat2& a, const mat2& b) {
	mat2 tmp = mat2();
	for (int i = 0; i < 4; i++)
		tmp.c[i] = a.c[i] + b.c[i];
	return tmp;
}

mat2 operator-(const mat2& a, const mat2& b) {
	mat2 tmp = mat2();
	for (int i = 0; i < 4; i++)
		tmp.c[i] = a.c[i] - b.c[i];
	return tmp;
}

mat2 operator*(const mat2& a, const mat2& b) {
	mat2 tmp = mat2();
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
				tmp.c[2 * i + j] += a.c[2 * i + k] * b.c[2 * k + j];
	return tmp;
}

vec2 operator*(const mat2& m, const vec2& v) {
	vec2 tmp = vec2();

	tmp.x = m.c[0] * v.x + m.c[1] * v.y;
	tmp.y = m.c[2] * v.x + m.c[3] * v.y;

	return tmp;
}

mat2 operator*(const mat2& m, float s) {
	mat2 tmp = mat2();
	for (int i = 0; i < 4; i++)
		tmp.c[i] = m.c[i] * s;
	return tmp;
}

mat2 operator/(const mat2& m, float s) {
	mat2 tmp = mat2();
	for (int i = 0; i < 4; i++)
		tmp.c[i] = m.c[i] / s;
	return tmp;
}

mat2 transpose(const mat2& m) {
	mat2 tmp = mat2();
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			tmp.c[2 * i + j] = m.c[2 * j + i];
	return tmp;
}

mat2 inverse(const mat2& m) {
	float det = m.c[0] * m.c[3] - m.c[1] * m.c[2];
	mat2 tmp = mat2(m.c[3], -m.c[1], -m.c[2], m.c[0]);
	return tmp / det;
}

mat2 operator*(float s, const mat2& m) {
	return m * s;
}
