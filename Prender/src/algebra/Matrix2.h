#pragma once

#include "../../main.h"
#include "Vector.h"

struct mat2
{
	float c[4];

	//!constructor from coeficient in line major
	mat2(float a11, float a12, float a21, float a22) {
		c[0] = a11;
		c[1] = a12;
		c[2] = a21;
		c[3] = a22;
	}

	//! construction from a list of coeficient in line major
	mat2(const float coefs[4]) {
		for (int i = 0; i < 4; i++)
			c[i] = coefs[i];
	}

	//! matrix 0
	mat2() {
		for (int i = 0; i < 4; i++)
			c[i] = 0;
	};

	//! matrix identity
	static mat2 identity() {
		return mat2(1, 0, 0, 1);
	}

	//! addition of matrix
	mat2* operator+=(const mat2& m);

	//! substraction of matrix
	mat2* operator-=(const mat2& m);

	//! matrix product
	mat2* operator*=(const mat2& m);

	//! multiplication by a scalar 
	mat2* operator*=(float s);

	//! division by a scalar
	mat2* operator/=(float s);
};

#ifdef CONSOLE
//! printing
std::ostream& operator<<(std::ostream& o, const mat2& m);
#endif

//! addition of matrixes
mat2 operator+(const mat2& a, const mat2& b);

//! substraction of matrixes
mat2 operator-(const mat2& a, const mat2& b);

//! matrix product
mat2 operator*(const mat2& a, const mat2& b);

//! multiplication of a vector by a matrix
vec2 operator*(const mat2& m, const vec2& v);

//! multiplication by a scalar
mat2 operator*(const mat2& m, float s);

//! multiplication by a scalar
mat2 operator*(float s, const mat2& m);

//! division by a scalar
mat2 operator/(const mat2& m, float s);

//! transposition of the matrix
mat2 transpose(const mat2& m);

//! inversion of the matrix
mat2 inverse(const mat2& m);
