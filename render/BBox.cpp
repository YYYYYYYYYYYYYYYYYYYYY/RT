#include "BBox.h"

CUDA_HD void BBox::join(Vector3& p)
{
	lower = minVector(lower, p);
	upper = maxVector(upper, p);
	size = upper - lower;
}

CUDA_HD void BBox::join(BBox& p)
{
	lower = minVector(lower, p.lower);
	upper = maxVector(upper, p.upper);
	size = upper - lower;
}


CUDA_HD bool BBox::checkIntersection(const Ray& r, float &tnear, float &tfar) const
{
#ifdef __CUDA_ARCH__
	float t[12];
	//t[0] = 1.0f / r.Dir.x();
	//t[1] = 1.0f / r.Dir.y();
	//t[2] = 1.0f / r.Dir.z();

	t[3] = (upper.e[0] - r.Start.e[0]) / r.Dir.x();
	t[4] = (lower.e[0] - r.Start.e[0]) / r.Dir.x();

	t[5] = (upper.e[1] - r.Start.e[1]) / r.Dir.y();
	t[6] = (lower.e[1] - r.Start.e[1]) / r.Dir.y();

	t[7] = (upper.e[2] - r.Start.e[2]) / r.Dir.z();
	t[8] = (lower.e[2] - r.Start.e[2]) / r.Dir.z();

	t[9] = fmax(fmax(fmin(t[3], t[4]), fmin(t[5], t[6])), fmin(t[7], t[8]));
	t[10] = fmin(fmin(fmax(t[3], t[4]), fmax(t[5], t[6])), fmax(t[7], t[8]));

	tfar = t[10];
	tnear = t[9];
	return !(t[10] < 0 || t[9] > t[10]);
#else
	__m128 d1 = _mm_sub_ps(upper.me, r.Start.me);
	__m128 d2 = _mm_sub_ps(lower.me, r.Start.me);

	d1 = _mm_div_ps(d1, r.Dir.me);
	d2 = _mm_div_ps(d2, r.Dir.me);

	__m128 fd1 = _mm_min_ps(d1, d2);
	__m128 fd2 = _mm_max_ps(d1, d2);
	tnear = max(fd1.m128_f32[0], max(fd1.m128_f32[1], fd1.m128_f32[2]));
	tfar = min(fd2.m128_f32[0], min(fd2.m128_f32[1], fd2.m128_f32[2]));

	return !(tfar < 0 || tnear > tfar);

#endif
}

CUDA_HD float BBox::surfaceArea() const
{
	return size.x() * size.y() + size.x() * size.z() + size.y() * size.z();
}

