#ifndef CYLINDER_H
#define CYLINDER_H
#include "BasicShape.h"

class Cylinder : public BasicShape
{
public:
	float Height;
	float Radius;
	float R2;

	CUDA_HD virtual void UpdateBB()
	{
		Vector3 p[8];
		p[0] = GetLocation() + X * GetScale().x() * Radius + Y * GetScale().y() * Radius;
		p[1] = GetLocation() + X * GetScale().x() * Radius + Y * GetScale().y() * Radius;

		p[2] = GetLocation() + X * GetScale().x() * Radius - Y * GetScale().y() * Radius;
		p[3] = GetLocation() + X * GetScale().x() * Radius - Y * GetScale().y() * Radius;

		p[4] = GetLocation() - X * GetScale().x() * Radius + Y * GetScale().y() * Radius + Z * GetScale().z() * Height;
		p[5] = GetLocation() - X * GetScale().x() * Radius + Y * GetScale().y() * Radius - Z * GetScale().z() * Height;

		p[6] = GetLocation() - X * GetScale().x() * Radius - Y * GetScale().y() * Radius + Z * GetScale().z() * Height;
		p[7] = GetLocation() - X * GetScale().x() * Radius - Y * GetScale().y() * Radius - Z * GetScale().z() * Height;

		bb = p[0];

		for (int i = 1; i < 8; i++)
		{
			bb.join(p[i]);
		}
		//printf("Lower: (%f, %f, %f), Upper: (%f, %f, %f)\n", BoundingLower.e[0], BoundingLower.e[1], BoundingLower.e[2], BoundingUpper.e[0], BoundingUpper.e[1], BoundingUpper.e[2]);
	}

	CUDA_HD Cylinder(float rad = 0.5f, float h = 2.0f, Vector3 Loc = Vector3(), Quaternion Rot = Quaternion(), Vector3 Sc = Vector3(1, 1, 1), Material Mat = Material(Vector3(0, 0, 1))) : BasicShape(Mat, Loc, Rot, Sc)
	{
		Height = h;
		Radius = rad;
		R2 = rad * rad;
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

		float a = r1.Dir.e[0] * r1.Dir.e[0] + r1.Dir.e[1] * r1.Dir.e[1];

		float b = 2 * (r1.Dir.e[0] * r1.Start.e[0] + r1.Dir.e[1] * r1.Start.e[1]);

		float c = r1.Start.e[0] * r1.Start.e[0] + r1.Start.e[1] * r1.Start.e[1] - R2;

		float d = b * b - 4 * a * c;

		//printf("a: %f, b: %f, c: %f\n", a, b, c);

		float d1 = sqrt(d);
		float t1 = (-b - d1) / 2.0f / a;
		float t2 = (-b + d1) / 2.0f / a;

		if (t1 >= 0)
		{
			Vector3 nPos = r1.Start + r1.Dir * t1;
			if ((0 <= nPos.e[2] && nPos.e[2] <= Height)) {

				Vector3 nNormal = nPos;
				nNormal.e[2] = 0;

				nNormal.normalize();

				nPos = nPos ^ GetScale();
				nPos = ToGlobal(nPos);
				nPos += GetLocation();

				nNormal = ToGlobal(nNormal);

				hitres.nPos = nPos;
				hitres.nNormal = nNormal;
				hitres.col = (1 + nNormal.e[colId]) / 2;
				hitres.dist = sqrt((nPos - r.Start).d2());
				//printf("dist: %f\n", hitres.dist);
				return 1;
			}
		}
		else if (t2 >= 0)
		{
			Vector3 nPos = r1.Start + r1.Dir * t2;
			if ((0 < nPos.e[2] && nPos.e[2] < Height)) {
				Vector3 nNormal = nPos;
				nNormal.e[2] = 0;

				nNormal.normalize();

				nPos = nPos ^ GetScale();
				nPos = ToGlobal(nPos);
				nPos += GetLocation();

				nNormal = ToGlobal(nNormal);

				hitres.nPos = nPos;
				hitres.nNormal = nNormal;
				hitres.col = nNormal.e[colId];
				hitres.dist = (nPos - r.Start).magnitude();
				return 1;
			}
		}

		bool up = false;
		bool down = false;

		float t_up = (Height - r1.Start.z()) / r1.Dir.z();
		float t_down = (0 - r1.Start.z()) / r1.Dir.z();

		float x_up = 0, y_up = 0;
		float x_down = 0, y_down = 0;

		x_up = r1.Start.x() + r1.Dir.x() * t_up;
		y_up = r1.Start.y() + r1.Dir.y() * t_up;
		up = (x_up*x_up + y_up * y_up < R2);
		
		x_down = r1.Start.x() + r1.Dir.x() * t_down;
		y_down = r1.Start.y() + r1.Dir.y() * t_down;
		down = (x_down*x_down + y_down * y_down < R2);
		// TODO: change colour
		if (!up && !down)
			return false;
		else {
			if (!down || t_up < t_down) {
				Vector3 nPos = r1.Start + r1.Dir * t_up;
				Vector3 nNormal = Vector3(0,0, 1);
				
				nPos = ToGlobal(nPos);
				nPos += GetLocation();
				nNormal.normalize();
				nNormal = ToGlobal(nNormal);

				hitres.nPos = nPos;
				hitres.nNormal = nNormal;
				hitres.col = 0.5 + (nPos.e[colId])/2;
				hitres.dist = (nPos - r.Start).magnitude();
			}
			else {
				Vector3 nPos = r1.Start + r1.Dir * t_down;
				Vector3 nNormal = Vector3(0,0,-1);
				
				nPos = ToGlobal(nPos);
				nPos += GetLocation();

				hitres.nPos = nPos;
				hitres.nNormal = nNormal;
				hitres.col = 0.5 + (nPos).e[colId]/2;
				hitres.dist = (nPos - r.Start).magnitude();
			}
			return 1;
		}
	}
};


#endif
