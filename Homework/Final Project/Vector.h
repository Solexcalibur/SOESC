#pragma once

#include <stdio.h>

class Vector {
public:
	Vector();
	Vector(float x, float y, float z);

	float length();
	void normalize(); //into unit vector

	Vector operator*(float scalar);
	void operator+=(const Vector& v);
	Vector operator-(const Vector& v);

	float x;
	float y;
	float z;
};
