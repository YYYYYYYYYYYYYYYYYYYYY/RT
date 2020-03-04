#ifndef SPHERE
#define SPHERE
#include "BasicShape.h"

class Sphere : public BasicShape
{
public:
	float Radius;
	float R2;

	CUDA_HD virtual void UpdateBB() 
	{
		Vector3 p[8];
		p[0] = GetLocation() + X * GetScale().x() * Radius + Y * GetScale().y() * Radius + Z * GetScale().z() * Radius;
		p[1] = GetLocation() + X * GetScale().x() * Radius + Y * GetScale().y() * Radius - Z * GetScale().z() * Radius;
		
		p[2] = GetLocation() + X * GetScale().x() * Radius - Y * GetScale().y() * Radius + Z * GetScale().z() * Radius;
		p[3] = GetLocation() + X * GetScale().x() * Radius - Y * GetScale().y() * Radius - Z * GetScale().z() * Radius;
		
		p[4] = GetLocation() - X * GetScale().x() * Radius + Y * GetScale().y() * Radius + Z * GetScale().z() * Radius;
		p[5] = GetLocation() - X * GetScale().x() * Radius + Y * GetScale().y() * Radius - Z * GetScale().z() * Radius;

		p[6] = GetLocation() - X * GetScale().x() * Radius - Y * GetScale().y() * Radius + Z * GetScale().z() * Radius;
		p[7] = GetLocation() - X * GetScale().x() * Radius - Y * GetScale().y() * Radius - Z * GetScale().z() * Radius;

		bb =  p[0];

		for (int i = 1; i < 8; i++)
		{
			bb.join(p[i]);
		}
	}

	CUDA_HD Sphere(float nRadius = 3.0f, Vector3 Loc = Vector3(), Quaternion Quat = Quaternion(), Vector3 Sc = Vector3(1, 1, 1), Material Mat = Material(Vector3(0, 1, 0))) : BasicShape(Mat, Loc, Quat, Sc)
	{
		Radius = nRadius;
		R2 = Radius * Radius;
		UpdateBB();
	}


	CUDA_HD virtual bool Culling(const Ray& r)
	{
		Vector3 Dist = GetLocation() - r.origin();
		return Dist * r.direction() <= 0.8;
	}
	CUDA_HD size_t size() override { return sizeof(Sphere); }
	CUDA_HD virtual bool hit(const Ray& r, HitResult& hitres, int colId)
	{
		//printf("in hit sphere\n");
		if (!__super::hit(r, hitres, colId)) return false;
		//if (Culling(r)) return false;
		
		Vector3 nOrigin = r.origin() - GetLocation();
		Vector3 nDir = r.origin() + r.direction() - GetLocation();

		nOrigin = ToLocal(nOrigin);
		nDir = ToLocal(nDir);

		nOrigin = nOrigin / GetScale();
		nDir = nDir / GetScale();

		nDir = nDir - nOrigin;
		nDir.normalize();

		float b = 2.0f * (nDir * nOrigin);

		float c = nOrigin.d2() - R2;

		float d = b * b - 4 * c;

		if (d < 0) return false;

		float t1 = (-b - sqrt(d)) / 2.0f;
		float t2 = (-b + sqrt(d)) / 2.0f;
		
		if (t1 >= 0)
		{
			Vector3 nPos = nOrigin + nDir * t1;
			Vector3 nNormal = nPos / GetScale();
			nPos = nPos | GetScale();
			nNormal.normalize();
			nPos = ToGlobal(nPos);
			nPos = nPos + GetLocation();
			hitres.nPos = nPos;
			hitres.nNormal = nNormal;
			hitres.dist = t1;
			hitres.col = (1 + nNormal.getByID(colId)) / 2.0f;
			return true;
		}
		else if (t2 >= 0)
		{
			Vector3 nPos = nOrigin + nDir * t2;
			Vector3 nNormal = nPos / GetScale();
			nPos = nPos ^ GetScale();
			nNormal.normalize();
			nPos = nPos ^ GetScale();
			nPos = ToGlobal(nPos);
			nPos = nPos + GetLocation();
			hitres.nPos = nPos;
			hitres.nNormal = nNormal;
			hitres.dist = t2;
			hitres.col = (1 + nNormal.getByID(colId)) / 2.0f;
			return true;
		}
		return false;
		/*

		Vector3 nst = r.Start - GetLocation();
		Vector3 ndr = nst + r.Dir;

		nst = ToLocal(nst);
		ndr = ToLocal(ndr);

		nst = nst / GetScale();
		ndr = ndr / GetScale();
		ndr = (ndr - nst).normal();

		Ray r1 = Ray(nst, ndr);

		
		Vector3 p1 = -r1.origin();
		if (p1.l2() > R2)
		{
			float v1 = p1 * r1.direction();
			if (v1 <= 0)
			{
				return false;
			}
			Vector3 d1 = p1 - r1.direction() * v1;
			float d = d1.l2();
			if (d > R2)
			{
				return false;
			}
			float v2 = sqrtf(R2 - d);
			
			Vector3 nPos = d1 - r1.direction() * v2;
			hitres.nNormal = nPos.normal();
			hitres.nNormal = hitres.nNormal / GetScale();

			hitres.nNormal = ToGlobal(hitres.nNormal);
			hitres.nNormal.normalize();
			

			hitres.nPos = nPos ^ GetScale();
			hitres.nPos = ToGlobal(hitres.nPos);
			hitres.col = (1 + hitres.nNormal.e[colId])/2.0;
			hitres.dist = (nPos - r.origin()).magnitude();
			return true;
		}
		else
		{
			return false;
		}
		*/
	}
	CUDA_D void hit1()
	{
		printf("hello sphere?");
	}
};

#endif