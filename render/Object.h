#ifndef OBJECT
#define  OBJECT
#include "Kernel.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "BBox.h"

struct HitResult
{
	float col;
	Vector3 nPos;
	Vector3 nNormal;
	float dist = 1e9;
	float maxDist = 1e4;
	uint16_t id;
};


class Object
{
public:
	Vector3 Location;
	Quaternion Rotation;
	Vector3 Scale;

	Vector3 X = Vector3(1, 0, 0);
	Vector3 Y = Vector3(0, 1, 0);
	Vector3 Z = Vector3(0, 0, 1);

	// 0 - lower point, 1 - upper point
	BBox bb;

	bool bAlive = 1;

	CUDA_HD Object(Vector3 Loc = Vector3(), Quaternion Rot = Quaternion(), Vector3 Sc = Vector3(1, 1, 1))
	{
		Location = Loc;
		Rotation = Rot;
		Scale = Sc;
		UpdateBB();
		bAlive = 1;
	}

	CUDA_HD inline void Remove()
	{
		bAlive = 0;
	}

	CUDA_HD inline Vector3 GetLocation() { return Location; }
	CUDA_HD inline Quaternion GetRotation() { return Rotation; }
	CUDA_HD inline Vector3 GetScale() { return Scale; }

	CUDA_HD virtual void UpdateBB() {};

	CUDA_HD inline void AddOffset(Vector3 Offset) { Location += Offset; UpdateBB(); }
	CUDA_HD inline void AddScale(Vector3 delta) { Scale += delta; UpdateBB(); }
	CUDA_HD inline void Rotate(Quaternion quat)
	{
		quat.Rotate(X);
		quat.Rotate(Y);
		quat.Rotate(Z);
		UpdateBB();
	}
	CUDA_HD inline Vector3 ToLocal(Vector3 Point)
	{
		return Vector3(Point * X, Point * Y, Point * Z);
	}

	CUDA_HD inline Vector3 ToGlobal(Vector3 Point)
	{
		return X * Point.x() + Y * Point.y() + Z * Point.z();
	}

	CUDA_HD inline void setLocation(Vector3 newLocation)
	{
		Location = newLocation;
		UpdateBB();
	}

	CUDA_HD inline void setScale(Vector3 newScale)
	{
		Scale = newScale;
		UpdateBB();
	}
};
#endif