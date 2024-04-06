#pragma once

#include "../../main.h"
#include "Vector.h"

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

	//! return the translation (assuming the the matrix represente some kind of translation)
	vec3 extractTranslation() const;

	//! return the rotation (assuming that the matrix represent some kind of rotation
	mat4 extractRotation() const;

	//! return the 3 scaling factors, assuming that the matrix represent some kind of scaling
	vec3 extractScale() const;
};

#ifdef CONSOLE
//! printing
std::ostream& operator<<(std::ostream& o, const mat4& m);
#endif

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

//! rotation of a radian around the X axis
mat4 rotationMatrixX(float a);

//! rotation of a radian around the Y axis
mat4 rotationMatrixY(float a);

//! rotation of a radian around the Z axis
mat4 rotationMatrixZ(float a);

//! translation by a vector v
mat4 translationMatrix(const vec3& v);

//! uniform scale of s
mat4 scaleMatrix(float s);

//! non uniform scale of (sX, sY, sZ)
mat4 scaleMatrix(const vec3& scales);

mat4 transformationMatrix(const vec3& rotations, const vec3& scales, const vec3& translation);

#ifdef IMGUI
struct ImGuiTransformationAttr;

struct ImGuiTransformationAttr
{
	vec3 translation;
	vec3 scales;
	mat4 OriginalRotation;// because it's unstable to get euler angle from a transformation, we keep the initial rotation as it is
	vec3 rotations;

	const char* name;

	//! extract the translation, scale and rotation from a transformation
	ImGuiTransformationAttr(const mat4& transformation);
	ImGuiTransformationAttr(const mat4& transformation, const char* name);

	//reset the attributes depending on the new transformation (without changing the rotations vector)
	void extract_transormations(const mat4& transformation);

	//! extract the translation, scale and rotation from a transformation close to the attributes
	void updateAttr(const mat4& transformation);

	mat4 getTansformation() const;

	void imGuiPrintAttribute();
};

#endif
