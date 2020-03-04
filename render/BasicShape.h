#ifndef SHAPE_H
#define SHAPE_H

#include "Kernel.h"
#include "Object.h"
#include "Material.h"
#include "Ray.h"
#include "RMath.h"

class BasicShape : public Object
{
private:
	bool isAlive;
	uint32_t link;
public:
	int32_t link1 = -1;
	Material Mat;
	CUDA_HD bool GetIsAlive()
	{
		return isAlive;
	}

	CUDA_HD uint32_t GetListNode()
	{
		return link;
	}

	CUDA_HD void setListNode(uint32_t nlink)
	{
		link = nlink;
	}

	CUDA_HD BasicShape(Material Mat1 = Material(), Vector3 Loc = Vector3(), Quaternion Rot = Quaternion(), Vector3 Scal = Vector3()) : Object(Loc, Rot, Scal)
	{
		Mat = Mat1;
		link = -1;
	}

	CUDA_HD virtual bool hit(const Ray& r, HitResult& hitres, int colId) { return bAlive; }
	/*
	__host__  void* operator new(size_t size)
	{
		void* ptr;
		cudaMallocManaged(&ptr, size);
		cudaDeviceSynchronize();
		return ptr;
	}
	__host__ void operator delete(void* ptr)
	{
		cudaDeviceSynchronize();
		cudaFree(ptr);
	}
	*/
	CUDA_HD virtual bool Culling(const Ray& r) { return false; }

	CUDA_HD virtual size_t size() { return sizeof(BasicShape); }

	CUDA_HD void kill() { isAlive = 0; }
};
#endif