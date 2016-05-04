#include "Vector.h"
#include <math.h>

using namespace std;

Vector::Vector()
{
}

Vector::Vector(float x, float y, float z) : x(x), y(y), z(z) {}

float Vector::length() {
	//return sqrt(powf(x, 2) + powf(y, 2) + powf(z, 2));
	return sqrt(x * x + y * y + z * z);
}

void Vector::normalize() {
	float length = this->length();

	x /= length;
	y /= length;
	z /= length;
}

Vector Vector::operator*(float scalar) {
	Vector v;
	v.x = x * scalar;
	v.y = y * scalar;
	v.z = z * scalar;

	return v;
}

void Vector::operator+=(const Vector& v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

Vector Vector::operator-(const Vector & v)
{
	Vector result;
	result.x = x - v.x;
	result.y = y - v.y;
	result.z = z - v.z;

	if (result.x < 0)
		result.x *= -1;
	else if (result.y < 0)
		result.y *= -1;
	else if (result.z < 0)
		result.z *= -1;


	return result;
}
