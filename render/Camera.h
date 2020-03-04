#ifndef CAMERA
#define CAMERA
#include "Object.h"

struct CameraParam
{
	char type; // 0 - perspective, 1 - no persepective, 2 - fish-eye
	float fov; // if type == 0.

	int Width, Height;

	Vector3 LeftCorner = Vector3(-1, 0, -1), Horizontal = Vector3(2, 0, 0), Vertical = Vector3(0, 0, 2);
	CUDA_HD CameraParam(char nType = 0, float nFOV = 90.0f, int nWidth = 1920, int nHeight = 1080)
	{
		type = nType;
		if (type == 0)
		{
			if (nFOV >= 180)
			{
				nFOV = 179;
			}
			if (nFOV < 45)
			{
				nFOV = 45;
			}
			float v = 2.0f / tanf(nFOV / 2.0f * acosf(-1) / 180.0f);
			LeftCorner.e[1] = v;
		}
		fov = nFOV;
		Height = nHeight;
		Width = nWidth;
	}
};

class Camera : public Object
{
public:
	CameraParam param;
	CUDA_HD Camera(CameraParam nParam = CameraParam(), Vector3 Loc = Vector3(), Quaternion Rot = Quaternion(), Vector3 Sc = Vector3()) : Object(Loc, Rot, Sc)
	{
		param = nParam;
	}
	CUDA_HD inline void dirAtPoint(Vector3& Start, Vector3& Direction, float u, float v)
	{
		if (param.type == 0)
		{
			Start = GetLocation();
			Direction = ToGlobal(param.LeftCorner + param.Horizontal * v + param.Vertical * u);
		}
		else if (param.type == 1)
		{
			Start = GetLocation() + X * (u - 0.5f) * param.Horizontal + Z * (v - 0.5f) * param.Vertical;
			Direction = Y;
		}
		else if (param.type == 2)
		{
			Start = GetLocation();
			float  alpha = param.fov * (v - 0.5f) * acosf(-1) / 180.0f;
			Vector3 pos = Vector3(3.0f * sinf(alpha), 3.0f * cosf(alpha), param.LeftCorner.z() + param.Vertical.z() * u);
			Direction = ToGlobal(pos).normal();
		}
		else if (param.type == 3)
		{
			Start = GetLocation();
			float alpha = param.fov * (v - 0.5f) * acosf(-1) / 180.0f;
			float beta = (u - 0.5f) * acosf(-1) / 3.0f;
			Vector3 pos = Vector3(sinf(alpha) * cosf(beta), cosf(alpha) * cosf(beta), sinf(beta));
			Direction = ToGlobal(pos).normal();
		}
		else if (param.type == 4)
		{
			Start = GetLocation() /*+ Z * ((u - 0.5) * 30)*/;
			float alpha = param.fov * (v - 0.5f) * acosf(-1) / 180.0f;
			float beta = (u - 0.5f) * acosf(-1) / 3.0f;
			Vector3 pos = X * sinf(alpha) + Y * cosf(alpha) + Z * sinf(beta) /* (1 + cosf(acosf(-1) * 1.5f * (v - 0.5)))*/;
			Direction = ToGlobal(pos).normal();
		}
		else
		{
			Start = GetLocation();
			float phi = param.fov * acosf(-1) / 360;
			Vector3 v1 = Start - X * sinf(phi) + Y * cosf(phi);
			
			Vector3 v2 = v1 + X * 2 * sinf(phi);

			float alpha = atanf(sinf(phi) * (v - 0.5f) / (cosf(phi) + 1));
			
			//float  alpha = param.fov * (v - 0.5f) * acosf(-1) / 180;
			Vector3 pos = Vector3(3.0f * sinf(alpha * 2), 3.0f * cosf(alpha * 2), param.LeftCorner.e[2] + param.Vertical.e[2] * u);
			Direction = ToGlobal(pos).normal();
			//printf("Direction at v=%f: (%f, %f, %f)\n", v, Direction.e[0], Direction.e[1], Direction.e[2]);
		}
	}
};

#endif