#ifndef RMATH_h
#define RMATH_h
#include <cmath>
#include "Vector3.h"
#define min(a, b) ((a) < (b)? (a) : (b))
#define max(a, b) ((a) > (b)? (a) : (b))

inline Vector3 minVector(Vector3 a, Vector3 b)
{
	return Vector3(min(a.x(), b.x()), min(a.y(), b.y()), min(a.z(), b.z()));
}

inline Vector3 maxVector(Vector3 a, Vector3 b)
{
	return Vector3(max(a.x(), b.x()), max(a.y(), b.y()), max(a.z(), b.z()));
}

#endif