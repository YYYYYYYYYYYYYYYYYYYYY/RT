#ifndef BVHTREE_H
#define BVHTREE_H
#include "Scene.h"
#include "BBox.h"

#define LB (2 * MAX_OBJECTS + 50)
#define LE (2 * MAX_OBJECTS + 51) 

struct BVHnode
{
	int32_t left = -1, right = -1, parent = -1;
	uint32_t link;
	uint16_t obj;
	uint32_t cnt = 0;
	BBox bb;
	int32_t depth;
	inline bool leaf() { return left == -1; }
	inline float SAH() { return bb.surfaceArea() * cnt; }
};

class BVHTree : public Scene
{
	BVHnode nodes[2 * MAX_OBJECTS + 50];
	ListNode freelist[2 * MAX_OBJECTS + 52];
	uint16_t ncnt = 0;

	int32_t root;
	CUDA_H int32_t insert(uint16_t link, int32_t id, int32_t rlink);
public:

	BVHTree() : Scene()
	{
		freelist[LB].next = LE;
		freelist[LE].prev = LB;
		root = -1;
		for (int i = LB - 1; i >= 0; i--)
		{
			freelist[i].link = i;
			fllistAddAfter(LB, i);
		}
	}
	CUDA_HD void BVHTree::fllistRemove(int32_t id);
	CUDA_HD void BVHTree::fllistAddAfter(uint32_t id, uint32_t toAdd);
	CUDA_H float nodesJoin(BVHnode& nd1, BVHnode& nd2);
	CUDA_H  void rebuild() override;
	CUDA_H int32_t rotateLeft(int32_t id);
	CUDA_H int32_t rotateRight(int32_t id);
	

	CUDA_H size_t getSize() override { return sizeof(*this); }

	CUDA_H void add(uint16_t link);

	CUDA_H void remove(int32_t id);
	CUDA_H bool CPU_traverse(HitResult& res, const Ray& r, int color, int cur);
	CUDA_HD bool CastRay(HitResult & res, const Ray& ray, int color, int32_t start = -1) override;
	CUDA_H void acceptPacket(Package* package, bool fastUpdate) override;
	CUDA_H void print() override;
	CUDA_H int dfs(int v);
};
#endif