#ifndef CRay
#define CRay
#include "Kernel.h"
#include "Vector3.h"

struct Ray
{
	Vector3 Start;
	Vector3 Dir;
	Vector3 invDir;
	float minDistance;
	float maxDistance;

	CUDA_HD Ray(Vector3 st, Vector3 dir, float minDistance = 0, float maxDistance = 1e9) : Start(st), Dir(dir.normal()), minDistance(minDistance), maxDistance(maxDistance) 
	{
		invDir = Vector3(1, 1, 1) / Dir;
	};
	CUDA_HD Vector3 origin() const { return Start;  }
	CUDA_HD Vector3 direction() const { return Dir; }
	CUDA_HD Vector3 PointAtTime(float t) const { return Start + Dir * t; }
};
#endif