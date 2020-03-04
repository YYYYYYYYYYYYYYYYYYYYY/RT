#ifndef QUAT
#define QUAT
#include "Kernel.h"
#include "Vector3.h"
#include <cmath>

class Quaternion
{
private:
	float e[4];
public:

	CUDA_HD Quaternion(float e0 = 0, float e1 = 0, float e2 = 0, float e3 = 0) { e[0] = e0; e[1] = e1; e[2] = e2; e[3] = e3; }
	CUDA_HD Quaternion(float a, Vector3 v) { e[0] = a; e[1] = v.x(); e[2] = v.y(); e[3] = v.z(); }
	CUDA_HD Quaternion(Vector3 v, float angle) { e[0] = cosf(angle / 2), e[1] = sinf(angle / 2) * v.x(), e[2] = sinf(angle / 2) * v.y(), e[3] = sinf(angle / 2) * v.z(); }
	CUDA_HD inline float w() const { return e[0]; }
	CUDA_HD inline float x() const { return e[1]; }
	CUDA_HD inline float y() const { return e[2]; }
	CUDA_HD inline float z() const { return e[3]; }

	CUDA_HD inline Quaternion operator-() { return Quaternion(-e[0], -e[1], -e[2], -e[3]); }
	CUDA_HD inline Quaternion operator+(const Quaternion &b) { return Quaternion(e[0] + b.e[0], e[1] + b.e[1], e[2] + b.e[2], e[3] + b.e[3]); }
	CUDA_HD inline Quaternion operator-(const Quaternion &b) { return Quaternion(e[0] - b.e[0], e[1] - b.e[1], e[2] - b.e[2], e[3] - b.e[3]); }
	CUDA_HD inline Quaternion operator*(const Quaternion &b) {
		return Quaternion(e[0] * b.e[0] - b.e[1] * e[1] - b.e[2] * e[2] - b.e[3] * e[3],
			e[1] * b.e[0] + e[0] * b.e[1] + e[2] * b.e[3] - b.e[2] * e[3],
			e[2] * b.e[0] + e[0] * b.e[2] + e[3] * b.e[1] - b.e[3] * e[1],
			e[3] * b.e[0] + e[0] * b.e[3] + e[1] * b.e[2] - b.e[1] * e[2]);
	}

	CUDA_HD inline Quaternion conjugate()
	{
		return Quaternion(e[0], -e[1], -e[2], -e[3]);
	}

	CUDA_HD inline Quaternion FromVector(const Vector3& Vec) { return Quaternion(0, Vec.x(), Vec.y(), Vec.z()); }

	CUDA_HD static inline Vector3 ToVector(const Quaternion& quat)
	{
		return Vector3(quat.x(), quat.y(), quat.z());
	}

	CUDA_HD Quaternion FromAngle(float alpha, Vector3 axis)
	{
		return Quaternion(cosf(alpha / 2), axis * sinf(alpha / 2));
	}

	CUDA_HD void Rotate(Vector3& Vec)
	{
		Quaternion Q1 = FromVector(Vec);
		Quaternion P1 = conjugate();
		Quaternion Q2 = *this * Q1 * P1;
		Vec = ToVector(Q2);
	}
};
#endif