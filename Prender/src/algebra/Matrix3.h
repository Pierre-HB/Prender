#pragma once

#include "../../main.h"
#include "Vector.h"

struct mat3
{
	float c[9];

	//!constructor from coeficient in line major
	mat3(float a11, float a12, float a13, float a21, float a22, float a23, float a31, float a32, float a33) {
		c[0] = a11;
		c[1] = a12;
		c[2] = a13;
		c[3] = a21;
		c[4] = a22;
		c[5] = a23;
		c[6] = a31;
		c[7] = a32;
		c[8] = a33;
	}

	//! construction from a list of coeficient in line major
	mat3(const float coefs[9]) {
		for (int i = 0; i < 9; i++)
			c[i] = coefs[i];
	}

	//! matrix 0
	mat3() {
		for (int i = 0; i < 9; i++)
			c[i] = 0;
	};

	//! matrix identity
	static mat3 identity() {
		return mat3(1, 0, 0, 0, 1, 0, 0, 0, 1);
	}

	//! addition of matrix
	mat3* operator+=(const mat3& m);

	//! substraction of matrix
	mat3* operator-=(const mat3& m);

	//! matrix product
	mat3* operator*=(const mat3& m);

	//! multiplication by a scalar 
	mat3* operator*=(float s);

	//! division by a scalar
	mat3* operator/=(float s);
};

#ifdef CONSOLE
//! printing
std::ostream& operator<<(std::ostream& o, const mat3& m);
#endif

//! addition of matrixes
mat3 operator+(const mat3& a, const mat3& b);

//! substraction of matrixes
mat3 operator-(const mat3& a, const mat3& b);

//! matrix product
mat3 operator*(const mat3& a, const mat3& b);

//! multiplication of a vector by a matrix
vec3 operator*(const mat3& m, const vec3& v);

//! multiplication by a scalar
mat3 operator*(const mat3& m, float s);

//! multiplication by a scalar
mat3 operator*(float s, const mat3& m);

//! division by a scalar
mat3 operator/(const mat3& m, float s);

//! transposition of the matrix
mat3 transpose(const mat3& m);

//! inversion of the matrix
mat3 inverse(const mat3& m);
