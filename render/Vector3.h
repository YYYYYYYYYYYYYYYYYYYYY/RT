#ifndef VECTOR_H
#define VECTOR_H
#include "Kernel.h"

CUDA_HD inline float frsqrt(float num)
{
	float x2 = num * 0.5F;
	float threehalfs = 1.5F;
	union
	{
		float f;
		unsigned int i;
	} conv = { num };

	conv.i = 0x5f3759df - (conv.i >> 1);
	conv.f *= (threehalfs - (x2 * conv.f * conv.f));
	return conv.f;
}


struct Vector3
{
	union
	{
		float e[4] = {0, 0, 0, 0};
		__m128 me;
	};
	CUDA_HD inline float x() const { return e[0]; };
	CUDA_HD inline float y() const { return e[1]; };
	CUDA_HD inline float z() const { return e[2]; };
	CUDA_HD inline float r() const { return e[0]; };
	CUDA_HD inline float g() const { return e[1]; };
	CUDA_HD inline float b() const { return e[2]; };
	
	CUDA_HD inline float getByID(int id) const { return e[id]; }

	CUDA_HD Vector3(float x = 0, float y = 0, float z = 0) { e[0] = x; e[1] = y, e[2] = z; }	
	CUDA_H Vector3(__m128 ve) { me = ve; }

	CUDA_HD inline Vector3 operator+(const Vector3 b) const 
	{
#ifdef __CUDA_ARCH__
		return Vector3(e[0] + b.e[0], e[1] + b.e[1], e[2] + b.e[2]); 
#else
		return Vector3(_mm_add_ps(me, b.me));
#endif
	}
	CUDA_HD inline Vector3 operator-(const Vector3 b) const {
#ifdef __CUDA_ARCH__
		return Vector3(e[0] - b.e[0], e[1] - b.e[1], e[2] - b.e[2]);
#else
		return Vector3(_mm_sub_ps(me, b.me));
#endif
	}
	CUDA_HD inline Vector3 operator*(const float b)   const 
	{ 
#ifdef __CUDA_ARCH__	
		return Vector3(e[0] * b, e[1] * b, e[2] * b);
#else
		return Vector3(_mm_mul_ps(me, _mm_set1_ps(b)));
#endif
	}
	CUDA_HD inline Vector3 operator/(const float b)   const 
	{
#ifdef __CUDA_ARCH__
		return Vector3(e[0] / b, e[1] / b, e[2] / b); 
#else
		return Vector3(_mm_div_ps(me, _mm_set1_ps(b)));
#endif
	}
	CUDA_HD inline Vector3 operator|(const Vector3& b)   const
	{
#ifdef __CUDA_ARCH__
		return Vector3(e[0] * b.e[0], e[1] * b.e[1], e[2] * b.e[2]);
#else
		return Vector3(_mm_mul_ps(me, b.me));
#endif
	}
	CUDA_HD inline Vector3 operator/(const Vector3 b) const 
	{
#ifdef __CUDA_ARCH__
		return Vector3(e[0] / b.e[0], e[1] / b.e[1], e[2] / b.e[2]);
#else
		return Vector3(_mm_div_ps(me, b.me));
#endif
	}
	CUDA_HD inline float operator*(const Vector3 b)   const 
	{
#ifdef __CUDA_ARCH__
		return e[0] * b.e[0] + e[1] * b.e[1] + e[2] * b.e[2]; 
#else
		__m128 tmp = _mm_mul_ps(me, b.me);
		return tmp.m128_f32[0] + tmp.m128_f32[1] + tmp.m128_f32[2];
#endif
	}
	CUDA_HD inline Vector3 operator^(const Vector3 b) const { return Vector3(e[1] * b.e[2] - e[2] * b.e[1], b.e[0] * e[2] - e[0] * b.e[2], e[0] * b.e[1] - e[1] * b.e[0]); }
	
	CUDA_HD inline Vector3 operator+=(const Vector3 b) 
	{ 
#ifdef __CUDA_ARCH__
		e[0] += b.e[0];
		e[1] += b.e[1];
		e[2] += b.e[2];
#else
		me = _mm_add_ps(me, b.me);
#endif
		return *this;
	}

	CUDA_HD inline Vector3 operator-=(const Vector3 b)
	{
#ifdef __CUDA_ARCH__
		e[0] -= b.e[0];
		e[1] -= b.e[1];
		e[2] -= b.e[2];
#else
		me = _mm_sub_ps(me, b.me);
#endif
		return *this;
	}

	CUDA_HD inline Vector3 operator*=(const float b) 
	{
#ifdef __CUDA_ARCH__
		e[0] *= b;
		e[1] *= b;
		e[2] *= b;
#else
		me = _mm_mul_ps(me, _mm_set1_ps(b));
#endif
		return *this;
	}

	CUDA_HD inline Vector3 operator/=(const float b)
	{
#ifdef __CUDA_ARCH__
		e[0] /= b;
		e[1] /= b;
		e[2] /= b;
#else
		me = _mm_div_ps(me, _mm_set1_ps(b));
#endif
return *this;
		return *this;
	}

	CUDA_HD inline float d2() const
	{
		return (*this) * (*this);
	}

	CUDA_HD inline Vector3 normal() const
	{
		/*float invlen = frsqrt(d2());
		return Vector3(e[0] * invlen, e[1] * invlen, e[2] * invlen);*/
		float len = sqrt(d2());
#ifdef __CUDA_ARCH__
		return Vector3(e[0] / len, e[1] / len, e[2] / len);
#else
		return Vector3(_mm_div_ps(me, _mm_set1_ps(len)));
#endif
	}

	CUDA_HD inline void normalize()
	{
#ifdef __CUDA_ARCH__
		float invlen = frsqrt(d2());
		e[0] *= invlen;
		e[1] *= invlen;
		e[2] *= invlen;
#else
		float len = sqrt(d2());
		me = _mm_div_ps(me, _mm_set1_ps(len));
#endif
	}

	CUDA_HD inline int colorToRGB() const { return (int(255.9f * e[0]) << 16) + (int(255.9f * e[1]) << 8) + int(255.9f * e[2]); }
	CUDA_HD inline int colorToBGR() const { return (int(255.9f * e[2]) << 16) + (int(255.9f * e[1]) << 8) + int(255.9f * e[0]); }
};
#endif