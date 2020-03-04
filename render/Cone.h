#ifndef CONE_H
#define CONE_H
#include "BasicShape.h" 

class Cone : public BasicShape
{

public: 
	float Height;
	float Radius;
	float R2;
	float angle;

	CUDA_HD Cone(float rad = 0.5f, float h = 2.0f, Vector3 Loc = Vector3(), Quaternion Rot = Quaternion(), Vector3 Sc = Vector3(1, 1, 1), Material Mat = Material(Vector3(1, 0, 0))) : BasicShape(Mat, Loc, Rot, Sc)
	{
		Radius = rad;
		Height = h;
		R2 = Radius * Radius;
		angle = Height * frsqrt(R2 + Height * Height);
	}
	CUDA_HD virtual bool hit(const Ray& r, HitResult& hitres, int colId)
	{
		Vector3 nst = r.Start - GetLocation();
		Vector3 ndr = nst + r.Dir;

		nst = ToLocal(nst);
		ndr = ToLocal(ndr);

		nst = nst / GetScale();
		ndr = ndr / GetScale();

		nst.e[2] -= Height;
		ndr.e[2] -= Height;

		Ray r1 = Ray(nst, (ndr - nst).normal());


	}

};

#endif // !CONE
