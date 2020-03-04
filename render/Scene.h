#ifndef SCENE1
#define SCENE1
#include "Kernel.h"
#include "iostream"
#include "stdlib.h"
#include "BasicShape.h"
#include <algorithm>
#include <assert.h>
#include <vector>
#include "Packet.h"
#include "Camera.h"


/*Links shortcut*/
#define FB (MAX_OBJECTS)
#define FE (MAX_OBJECTS + 1)
#define TB (MAX_OBJECTS + 2)
#define TE (MAX_OBJECTS + 3)


struct ListNode
{
	uint32_t next = -1;
	uint32_t prev = -1;
	uint32_t link = 0;
};

class Scene
{
protected:
	BasicShape* sp[MAX_OBJECTS];
	uint16_t cnt;
	
	ListNode shapeNodes[MAX_OBJECTS + 4];
public:
	int32_t iC = 0;
	CUDA_HD Scene()
	{
		cnt = 0;
		shapeNodes[FB].next = FE;
		shapeNodes[FE].prev = FB;
		shapeNodes[TB].next = TE;
		shapeNodes[TE].prev = TB;
	}
	
	CUDA_HD inline BasicShape* getShapeByID(uint16_t id) { return sp[id]; }

	CUDA_HD void splistRemove(uint32_t id);
	CUDA_HD void splistAddAfter(uint32_t id, uint32_t toAdd);

	CUDA_HD ~Scene()
	{

	}

	CUDA_H int addElem(BasicShape* nSp);
	CUDA_H void removeElem(int id);
	CUDA_H void initScene(std::vector<BasicShape*> bsv);

	CUDA_H  virtual void rebuild();
	CUDA_H  virtual size_t getSize();
	
	CUDA_HD virtual bool CastRay(HitResult & res, const Ray& ray, int color, int32_t start = -1);

	CUDA_D void clear();

	CUDA_H virtual void print() {};

	/*CUDA_HD bool PointInNode(int id, Vector3 Point);
	CUDA_HD float CalcDirectionalLight(Ray r, int col, int start);
	CUDA_HD int Traverse(int id, Ray r);
	CUDA_HD bool FindIntersection(const Ray& r, TraceInfo& info, int id, HitResult& res, int colId);
	CUDA_HD int Move(const Ray& r, int id, float& accdist);
	CUDA_HD int RayCast(Ray r, int colId, HitResult& res, int start);
	*/

	CUDA_H virtual void acceptPacket(Package* package, bool fastUpdate);
	CUDA_H void getSceneObjects(std::vector<BasicShape*>& objs);

	CUDA_H void printBenchmark();

#ifdef CUDA
	CUDA_H void moveToCUDA(Scene** gScene);
#endif
};

#endif