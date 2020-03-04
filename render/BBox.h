#ifndef BBoxH
#define BBoxH
#include "Vector3.h"
#include "Ray.h"
#include "RMath.h"

struct BBox
{

	Vector3 lower, upper;
	Vector3 size;

	CUDA_HD BBox(Vector3 lower = Vector3(), Vector3 upper = Vector3()) : lower(lower), upper(upper) {};
	CUDA_HD BBox(BBox& bb) : lower(bb.lower), upper(bb.upper) {};
	CUDA_HD inline BBox operator=(Vector3& a) { lower = upper = a; return *this; }
	CUDA_HD void join(Vector3& p);
	CUDA_HD void join(BBox& p);
	CUDA_HD bool checkIntersection(const Ray& r, float &tnear, float &tfar) const;
	CUDA_HD float surfaceArea() const;

};

#endif