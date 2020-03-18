#ifndef BOX_H
#define BOX_H
#include "BasicShape.h"

class Box : public BasicShape
{
public:

	float A, B, C;

	CUDA_HD virtual void UpdateBB()
	{
		Vector3 p[8];
		p[0] = (GetLocation() + X * GetScale().x() * A + Y * GetScale().y() * B);
		p[1] = GetLocation() + X * GetScale().x() * A;

		p[2] = GetLocation() + X * GetScale().x() * A + Z * GetScale().y() * C;
		p[3] = GetLocation() + Y * GetScale().y() * B;

		p[4] = GetLocation() + Y * GetScale().y() * B + Z * GetScale().z() * C;
		p[5] = GetLocation();

		p[6] = GetLocation() + Z * GetScale().z() * C;
		p[7] = GetLocation() + X * GetScale().x() * A + Y * GetScale().y() * B + Z * GetScale().z() * C;
		bb = p[0];

		for (int i = 1; i < 8; i++)
		{
			bb.join(p[i]);
		}
		//printf("Lower: (%f, %f, %f), Upper: (%f, %f, %f)\n", BoundingLower.e[0], BoundingLower.e[1], BoundingLower.e[2], BoundingUpper.e[0], BoundingUpper.e[1], BoundingUpper.e[2]);
	}

	CUDA_HD Box(float a = 1.0f, float b = 1.0f, float c = 1.0f,Vector3 Loc = Vector3(), Quaternion Rot = Quaternion(), Vector3 Sc = Vector3(1, 1, 1), Material Mat = Material(Vector3(0, 0, 1))) : BasicShape(Mat, Loc, Rot, Sc)
	{
		A = a;
		B = b;
		C = c;
		UpdateBB();
	}

	CUDA_HD virtual bool hit(const Ray& r, HitResult& hitres, int colId)
	{	
		Vector3 nst = r.Start - GetLocation();
		Vector3 ndr = nst + r.Dir;
		nst = ToLocal(nst);
		ndr = ToLocal(ndr);

		nst = nst / GetScale();
		ndr = ndr / GetScale();

		Ray r1 = Ray(nst, (ndr - nst).normal());

		float t[6];
		bool hit[6];
		t[0] = (C - r1.Start.z()) / r1.Dir.z();
		t[1] = (0 - r1.Start.z()) / r1.Dir.z();

		float x = 0, y = 0, z = 0;

		x = r1.Start.x() + r1.Dir.x() * t[0];
		y = r1.Start.y() + r1.Dir.y() * t[0];
		hit[0] = (0 < x && x < A && 0 < y && y < B);

		x = r1.Start.x() + r1.Dir.x() * t[1];
		y = r1.Start.y() + r1.Dir.y() * t[1];
		hit[1] = (0 < x && x < A && 0 < y && y < B);

		t[2] = (0 - r1.Start.x()) / r1.Dir.x();
		t[3] = (A - r1.Start.x()) / r1.Dir.x();

		z = r1.Start.z() + r1.Dir.z() * t[2];
		y = r1.Start.y() + r1.Dir.y() * t[2];
		hit[2] = (0 < z && z < C && 0 < y && y < B);

		z = r1.Start.z() + r1.Dir.z() * t[3];
		y = r1.Start.y() + r1.Dir.y() * t[3];
		hit[3] = (0 < z && z < C && 0 < y && y < B);

		t[4] = (0 - r1.Start.y()) / r1.Dir.y();
		t[5] = (B - r1.Start.y()) / r1.Dir.y();

		z = r1.Start.z() + r1.Dir.z() * t[4];
		x = r1.Start.x() + r1.Dir.x() * t[4];
		hit[4] = (0 < z && z < C && 0 < x && x < A);

		x = r1.Start.x() + r1.Dir.x() * t[5];
		z = r1.Start.z() + r1.Dir.z() * t[5];
		hit[5] = (0 < z && z < C && 0 < x && x < A);
		// TODO : change colour and normal

		// here we count nearest edge
		int e = -1;

		for (int j = 0; j < 6; ++j) {
			if (hit[j]) {
				e = j;
				break;
			}
		}
		for (int j = e; j < 6; ++j) {
			if (hit[j] && abs(t[j]) < abs(t[e]))
				e = j;
		}
		///

		if (e == -1)
			return false;
		else {
			Vector3 nPos = r1.Start + r1.Dir * t[e];

			nPos = ToGlobal(nPos);
			nPos += GetLocation();

			hitres.nPos = nPos;
			hitres.dist = (nPos - r.Start).magnitude();

			switch (e)
			{
			case -1:
				return false;
			case 0: {
				Vector3 nNormal = Vector3(0, 0, 1);
				nNormal = ToGlobal(nNormal);
				hitres.nNormal = nNormal;
				hitres.col = 200;
				hitres.dist = (nPos - r.Start).magnitude();
				return 1;
			}
			case 1:
			{
				Vector3 nNormal = Vector3(0, 0, -1);
				nNormal = ToGlobal(nNormal);
				hitres.nNormal = nNormal;
				hitres.col = 10;
				hitres.dist = (nPos - r.Start).magnitude();
				return 1;
			}
			case 2: {
				Vector3 nNormal = Vector3(-1, 0, 0);
				nNormal = ToGlobal(nNormal);
				hitres.nNormal = nNormal;
				hitres.col = 120;
				hitres.dist = (nPos - r.Start).magnitude();
				return 1;
			}
			case 3:
			{
				Vector3 nNormal = Vector3(-1, 0, 0);
				nNormal = ToGlobal(nNormal);
				hitres.nNormal = nNormal;
				hitres.col = 180;
				hitres.dist = (nPos - r.Start).magnitude();
				return 1;
			}
			case 4:
			{
				Vector3 nNormal = Vector3(0, 1, 0);
				nNormal = ToGlobal(nNormal);
				hitres.nNormal = nNormal;
				hitres.col = 90;
				hitres.dist = (nPos - r.Start).magnitude();
				return 1;
			}
			case 5:
			{
				Vector3 nNormal = Vector3(0, -1, 0);
				nNormal = ToGlobal(nNormal);
				hitres.nNormal = nNormal;
				hitres.col = 20;
				hitres.dist = (nPos - r.Start).magnitude();
				return 1;
			}
			}
		}
	}

};


#endif
