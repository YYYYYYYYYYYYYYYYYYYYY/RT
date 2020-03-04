#ifndef RENDERER_H
#define RENDERER_H

#include "Kernel.h"
#include "Scene.h"
#include "BVHTree.h"
class Renderer
{
#ifdef CUDA
private:
	bool updated = true;
#endif

public:

	Scene* scene;
#ifdef CUDA
	Scene* gScene = nullptr;
#endif

	Camera cam;

	CUDA_H Renderer(CameraParam param)
	{
		scene = /*new Scene();*/ new BVHTree();
		cam = Camera(param);
	}

	void initScene(std::vector<BasicShape*> bsp) { scene->initScene(bsp); }

	CUDA_HD float TraceRay(Ray initial, int32_t& fId, int colId, int start, int maxDepth);

	void CPURenderPPMImage(uint8_t **image, int32_t **ids, size_t width, size_t height);
	void printScene() { scene->print(); };
#ifdef CUDA
	CUDA_H void CUDARenderPPMImage(uint8_t **image, int32_t **ids, size_t width, size_t height);
	__global__ void dRender(uint8_t * image, int32_t *ids, size_t width, size_t height, Camera cam);
#endif

};
#endif