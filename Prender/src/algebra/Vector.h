#pragma once

#include <vector>
#include "../../main.h"


template<typename T>
struct vector2;

typedef vector2<float> vec2;
typedef vector2<int> ivec2;

template<typename T>
struct vector3;

typedef vector3<float> vec3;
typedef vector3<int> ivec3;

template<typename T>
struct vector4;

typedef vector4<float> vec4;
typedef vector4<int> ivec4;


template<typename T>
struct vector2
{
	T x;
	T y;

	vector2(T x, T y) : x(x), y(y) {

	}

	vector2() : x(), y() {

	}

	vector2* operator +=(const vector2& b) {
		x += b.x;
		y += b.y;
		return this;
	}

	vector2* operator -=(const vector2& b) {
		x -= b.x;
		y -= b.y;
		return this;
	}

	vector2* operator *=(const vector2& b) {
		x *= b.x;
		y *= b.y;
		return this;
	}

	vector2* operator /=(const vector2& b) {
		x /= b.x;
		y /= b.y;
		return this;
	}

	vector2* operator *=(const T& b) {
		x *= b;
		y *= b;
		return this;
	}

	vector2* operator /=(const T& b) {
		x /= b;
		y /= b;
		return this;
	}
};

#ifdef CONSOLE
//! printing of vecctor2
template<typename T>
std::ostream& operator<<(std::ostream& o, const vector2<T>& v) {
	o << "(" << v.x << ", " << v.y << ")";
	return o;
}
#endif

//! Addition of vec2
template<typename T>
vector2<T> operator+(const vector2<T>& a, const vector2<T>& b) {
	return vector2<T>(a.x + b.x, a.y + b.y);
}

//! Susbtraction of vec2
template<typename T>
vector2<T> operator-(const vector2<T>& a, const vector2<T>& b) {
	return vector2<T>(a.x - b.x, a.y - b.y);
}

//! Negation of vec2
template<typename T>
vector2<T> operator-(const vector2<T>& v) {
	return vector2<T>(-v.x, -v.y);
}

//! Multiplication componentwise of vec2
template<typename T>
vector2<T> operator*(const vector2<T>& a, const vector2<T>& b) {
	return vector2<T>(a.x * b.x, a.y * b.y);
}

//! Multiplication by a scalar
template<typename T>
vector2<T> operator*(const vector2<T>& a, const T& b) {
	return vector2<T>(a.x * b, a.y * b);
}

//! Multiplication by a scalar
template<typename T>
vector2<T> operator*(const T& a, const vector2<T>& b) {
	return b*a;
}

//! Division componentwise of vec2
template<typename T>
vector2<T> operator/(const vector2<T>& a, const vector2<T>& b) {
	return vector2<T>(a.x / b.x, a.y / b.y);
}

//! Division componentwise of vec2
template<typename T>
vector2<T> operator/(const vector2<T>& a, const T& b) {
	return vector2<T>(a.x / b, a.y / b);
}

//! dot product of vec2
template<typename T>
T dot(const vector2<T>& a, const vector2<T>& b) {
	return a.x * b.x + a.y * b.y;
}

//! squared length of a vec2
template<typename T>
T length2(const vector2<T>& a) {
	return dot(a, a);
}

//! length of a vec2
template<typename T>
T length(const vector2<T>& a) {
	return sqrt(dot(a, a));
}

//! length of a vector4
template<typename T>
vector2<T> normalize(const vector2<T>& a) {
	return a / length(a);
}



template<typename T>
struct vector3
{
	T x;
	T y;
	T z;

	vector3(T x, T y, T z) : x(x), y(y), z(z) {

	}

	vector3() : x(), y(), z() {

	}

	vector3(const vector2<T>& v) : x(v.x), y(v.y), z() {

	}

	vector3(const vector2<T>& v, const T& z) : x(v.x), y(v.y), z(z) {

	}

	vector3* operator +=(const vector3& b) {
		x += b.x;
		y += b.y;
		z += b.z;
		return this;
	}

	vector3* operator -=(const vector3& b) {
		x -= b.x;
		y -= b.y;
		z -= b.z;
		return this;
	}

	vector3* operator *=(const vector3& b) {
		x *= b.x;
		y *= b.y;
		z *= b.z;
		return this;
	}

	vector3* operator /=(const vector3& b) {
		x /= b.x;
		y /= b.y;
		z /= b.z;
		return this;
	}

	vector3* operator *=(const T& b) {
		x *= b;
		y *= b;
		z *= b;
		return this;
	}

	vector3* operator /=(const T& b) {
		x /= b;
		y /= b;
		z /= b;
		return this;
	}
};

#ifdef CONSOLE
//! printing of vector3
template<typename T>
std::ostream& operator<<(std::ostream& o, const vector3<T>& v) {
	o << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return o;
}
#endif

//! Addition of vector3
template<typename T>
vector3<T> operator+(const vector3<T>& a, const vector3<T>& b) {
	return vector3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

//! Susbtraction of vector3
template<typename T>
vector3<T> operator-(const vector3<T>& a, const vector3<T>& b) {
	return vector3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

//! Negation of vec3
template<typename T>
vector3<T> operator-(const vector3<T>& v) {
	return vector3<T>(-v.x, -v.y, -v.z);
}

//! Multiplication componentwise of vector3
template<typename T>
vector3<T> operator*(const vector3<T>& a, const vector3<T>& b) {
	return vector3<T>(a.x * b.x, a.y * b.y, a.z * b.z);
}

//! Vector product of vector3
template<typename T>
vector3<T> operator^(const vector3<T>& a, const vector3<T>& b) {
	T x = a.y * b.z - a.z * b.y;
	T y = a.z * b.x - a.x * b.z;
	T z = a.x * b.y - a.y * b.x;
	return vector3<T>(x, y, z);
}

//! Multiplication by a scalar
template<typename T>
vector3<T> operator*(const vector3<T>& a, const T& b) {
	return vector3<T>(a.x * b, a.y * b, a.z * b);
}

//! Multiplication by a scalar
template<typename T>
vector3<T> operator*(const T& a, const vector3<T>& b) {
	return b * a;
}

//! Division componentwise of vector3
template<typename T>
vector3<T> operator/(const vector3<T>& a, const vector3<T>& b) {
	return vector3<T>(a.x / b.x, a.y / b.y, a.z / b.z);
}

//! Division componentwise of vector3
template<typename T>
vector3<T> operator/(const vector3<T>& a, const T& b) {
	return vector3<T>(a.x / b, a.y / b, a.z / b);
}

//! dot product of vector3
template<typename T>
T dot(const vector3<T>& a, const vector3<T>& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

//! squared length of a vector3
template<typename T>
T length2(const vector3<T>& a) {
	return dot(a, a);
}

//! length of a vector3
template<typename T>
T length(const vector3<T>& a) {
	return sqrtf(dot(a, a));
}

//! length of a vector4
template<typename T>
vector3<T> normalize(const vector3<T>& a) {
	return a / length(a);
}




template<typename T>
struct vector4
{
	T x;
	T y;
	T z;
	T w;

	vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {

	}

	vector4() : x(), y(), z(), w() {

	}

	vector4(const vector2<T>& v) : x(v.x), y(v.y), z(), w() {

	}

	vector4(const vector2<T>& v, const T& z, const T& w) : x(v.x), y(v.y), z(z), w(w) {

	}

	vector4(const vector3<T>& v) : x(v.x), y(v.y), z(v.z), w() {

	}

	vector4(const vector3<T>& v, const T& w) : x(v.x), y(v.y), z(v.z), w(w) {

	}

	vector4* operator +=(const vector4& b) {
		x += b.x;
		y += b.y;
		z += b.z;
		w += b.w;
		return this;
	}

	vector4* operator -=(const vector4& b) {
		x -= b.x;
		y -= b.y;
		z -= b.z;
		w -= b.w;
		return this;
	}

	vector4* operator *=(const vector4& b) {
		x *= b.x;
		y *= b.y;
		z *= b.z;
		w *= b.w;
		return this;
	}

	vector4* operator /=(const vector4& b) {
		x /= b.x;
		y /= b.y;
		z /= b.z;
		w /= b.w;
		return this;
	}

	vector4* operator *=(const T& b) {
		x *= b;
		y *= b;
		z *= b;
		w *= b;
		return this;
	}

	vector4* operator /=(const T& b) {
		x /= b;
		y /= b;
		z /= b;
		w /= b;
		return this;
	}
};

#ifdef CONSOLE
//! printing of vector4
template<typename T>
std::ostream& operator<<(std::ostream& o, const vector4<T>& v) {
	o << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	return o;
}
#endif

//! Addition of vector4
template<typename T>
vector4<T> operator+(const vector4<T>& a, const vector4<T>& b) {
	return vector4<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

//! Susbtraction of vector4
template<typename T>
vector4<T> operator-(const vector4<T>& a, const vector4<T>& b) {
	return vector4<T>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

//! Negation of vec4
template<typename T>
vector4<T> operator-(const vector4<T>& v) {
	return vector4<T>(-v.x, -v.y, -v.z, -v.w);
}

//! Multiplication componentwise of vector4
template<typename T>
vector4<T> operator*(const vector4<T>& a, const vector4<T>& b) {
	return vector4<T>(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

//! Multiplication by a scalar
template<typename T>
vector4<T> operator*(const vector4<T>& a, const T& b) {
	return vector4<T>(a.x * b, a.y * b, a.z * b, a.w * b);
}

//! Multiplication by a scalar
template<typename T>
vector4<T> operator*(const T& a, const vector4<T>& b) {
	return b * a;
}

//! Division componentwise of vector4
template<typename T>
vector4<T> operator/(const vector4<T>& a, const vector4<T>& b) {
	return vector4<T>(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

//! Division componentwise of vector4
template<typename T>
vector4<T> operator/(const vector4<T>& a, const T& b) {
	return vector4<T>(a.x / b, a.y / b, a.z / b, a.w / b);
}

//! dot product of vector4
template<typename T>
T dot(const vector4<T>& a, const vector4<T>& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

//! squared length of a vector4
template<typename T>
T length2(const vector4<T>& a) {
	return dot(a, a);
}

//! length of a vector4
template<typename T>
T length(const vector4<T>& a) {
	return sqrt(dot(a, a));
}

//! length of a vector4
template<typename T>
vector4<T> normalize(const vector4<T>& a) {
	return a/length(a);
}

