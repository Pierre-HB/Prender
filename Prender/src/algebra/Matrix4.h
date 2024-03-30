#pragma once

#include "Vector.h"
#include <ostream>

struct mat4
{
	float c[16];

	//!constructor from coeficient in line major
	mat4(float a11, float a12, float a13, float a14, float a21, float a22, float a23, float a24, float a31, float a32, float a33, float a34, float a41, float a42, float a43, float a44) {
		c[0 ] = a11;
		c[1 ] = a12;
		c[2 ] = a13;
		c[3 ] = a14;
		c[4 ] = a21;
		c[5 ] = a22;
		c[6 ] = a23;
		c[7 ] = a24;
		c[8 ] = a31;
		c[9 ] = a32;
		c[10] = a33;
		c[11] = a34;
		c[12] = a41;
		c[13] = a42;
		c[14] = a43;
		c[15] = a44;

	}

	//! construction from a list of coeficient in line major
	mat4(const float coefs[16]) {
		for (int i = 0; i < 16; i++)
			c[i] = coefs[i];
	}

	//! matrix 0
	mat4() {
		for (int i = 0; i < 16; i++)
			c[i] = 0;
	};

	//! matrix identity
	static mat4 identity() {
		return mat4(1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1);
	}

	//! addition of matrix
	mat4* operator+=(const mat4& m);

	//! substraction of matrix
	mat4* operator-=(const mat4& m);

	//! matrix product
	mat4* operator*=(const mat4& m);

	//! multiplication by a scalar 
	mat4* operator*=(float s);

	//! division by a scalar
	mat4* operator/=(float s);
};

//! printing
std::ostream& operator<<(std::ostream& o, const mat4& m);

//! addition of matrixes
mat4 operator+(const mat4& a, const mat4& b);

//! substraction of matrixes
mat4 operator-(const mat4& a, const mat4& b);

//! matrix product
mat4 operator*(const mat4& a, const mat4& b);

//! multiplication of a vector by a matrix
vec4 operator*(const mat4& m, const vec4& v);

//! multiplication in homogeneous space of a vec3 and projection back into te real space by dividing by w
vec3 operator*(const mat4& m, const vec3& v);

//! multiplication by a scalar
mat4 operator*(const mat4& m, float s);

//! multiplication by a scalar
mat4 operator*(float s, const mat4& m);

//! division by a scalar
mat4 operator/(const mat4& m, float s);

//! transposition of the matrix
mat4 transpose(const mat4& m);

//! inversion of the matrix
mat4 inverse(const mat4& m);

//! Transformation matrix for he normals (so that normals stay perpendicular to the surface)
mat4 normalTransformation(const mat4& m);



//! rotation of a radian around the Y axis
mat4 rotationMatrixY(float a);

mat4 translationMatrix(const vec3& v);
//! viewport matrix
//mat4 viewPort(int width, int height);

//lookat

//viewport

//projection perspective

//projection ortogonal

//scale

//scale X, ...
//translate
// rptate around a vector v
//rotate X, ...




//TODO Rotation with mat3
//TODO mat4(mat3) (the upper left part of the matrix, the rotation part)