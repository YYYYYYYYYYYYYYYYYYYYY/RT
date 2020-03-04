#ifndef MAT_H
#define MAT_H

#include "Kernel.h"
#include "Vector3.h"

class Material
{
public:
	Vector3 Coefs;
	Vector3 Color;
	float OptFlat;
	CUDA_HD Material(Vector3 Colors = Vector3(0.1f, 1.0f, .3f), Vector3 Coef = Vector3(1.0f, 0, 0), float OptF = 1.0f)
	{
		Coefs = Coef;
		Color = Colors;
		OptFlat = OptFlat;
	}

	CUDA_HD float GetDiffuse() { return Coefs.x(); }
	CUDA_HD float GetReflect() { return Coefs.y(); }
	CUDA_HD float GetRefract() { return Coefs.z(); }
};

#endif