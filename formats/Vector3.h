#include <cmath>
#pragma once


class Vector3
{	 
public:
	double x, y, z;
	Vector3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {};
	Vector3 operator+(Vector3 b)
	{
		return Vector3(x + b.x, y + b.y, z + b.z);
	}
	Vector3 operator-()
	{
		return Vector3(-x, -y, -z);
	}
	Vector3 operator-(Vector3 b)
	{
		return Vector3(x - b.x, y - b.y, z - b.z);
	}
	double operator*(Vector3 b)
	{
		return x * b.x + y * b.y + z * b.z;
	}
	Vector3 operator*(double b)
	{
		return Vector3(x * b, y * b, z * b);
	}
	Vector3 normal()
	{
		return Vector3(x / magnitude(), y / magnitude(), z / magnitude());
	}
	double magnitude()
	{
		return sqrt(x*x + y*y + z*z);
	}
};