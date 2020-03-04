#include "Renderer.h"
#include <ctime>

void Renderer::CPURenderPPMImage(uint8_t** image, int32_t **ids, size_t width, size_t height)
{	
	clock_t t1 = clock();
	size_t id = 0;
	for (size_t i = 0/*height/2*/; i < height; i++)
	{
		for (size_t j = 0/*width/2*/; j < width; j++)
		{
			float dy = i * 1.0f / height;
			float dx = j * 1.0f / width;
			Vector3 color = Vector3(255.5f * dy, 255.5f * dx, 255.5f * dy * dx);
			Ray r = Ray(Vector3(), Vector3());
			cam.dirAtPoint(r.Start, r.Dir, 1 - dy, dx);
			color.e[0] = TraceRay(r, (*ids)[id/3], 0, 0, 2);
			color.e[1] = TraceRay(r, (*ids)[id/3], 1, 0, 2);
			color.e[2] = TraceRay(r, (*ids)[id/3], 2, 0, 2);
			(*image)[id] = uint8_t(255.5 * color.e[0]);
			(*image)[id + 1] = uint8_t(255.5 * color.e[1]);
			(*image)[id + 2] = uint8_t(255.5 * color.e[2]);
			id += 3;
		}
	}
	clock_t t2 = clock();
	std::cerr << "Time to render: " << (t2 - t1) * 1.0 / CLOCKS_PER_SEC << std::endl;
}
#ifdef CUDA
CUDA_H void Renderer::CUDARenderPPMImage(uint8_t ** image, int32_t **ids, size_t width, size_t height)
{
	if (!updated)
	{
		if (gScene != nullptr)
		{
			gScene->clear();
			cudaFree(gScene);
		}
		cudaMalloc(&gScene, scene->getSize());
		cudaMemcpy(&gScene, scene, scene->getSize(), cudaMemcpyHostToDevice);
		scene->moveToCUDA(&gScene);
	}


	//dRender<<
}
__global__ void Renderer::dRender(uint8_t * image, int32_t * ids, size_t width, size_t height, Camera cam)
{
	/*int i = threadIdx.x + blockIdx.x * blockDim.x;
	int j = threadIdx.y + blockIdx.y * blockDim.y;
	if (i >= cam.param.Height || j >= cam.param.Width) return;
	int id = (i * cam.param.Width + j);

	float u = float(i) / float(cam.param.Height);
	float v = float(j) / float(cam.param.Width);

	Ray r = Ray(Vector3(), Vector3());
	cam.dirAtPoint(r.Start, r.Dir, u, v);

	image[id * 3] = TraceRay(r, ids[id], 0, -1, 5);
	image[id * 3 + 1] = TraceRay(r, ids[id], 1, -1, 5);
	image[id * 3 + 2] = TraceRay(r, ids[id], 2, -1, 5);*/
}
#endif

CUDA_HD float Renderer::TraceRay(Ray initial, int32_t& fId, int colId, int start, int maxDepth)
{
	float res = 0;
	float value = 1;

	HitResult hit;

	Ray current = initial;
	fId = -1;
	if (!scene->CastRay(hit, current, colId))
	{
		Vector3 dir = current.direction();
		float t = 0.5f * (dir.y() + 1.0f);
		return (Vector3(1.0f, 1.0f, 1.0f) * (1.0f - t) + Vector3(0.5f, 0.7f, 1.0f) * t).e[colId];
	}
	fId = hit.id;
	BasicShape* ht = scene->getShapeByID(hit.id);

	res += hit.col * ht->Mat.GetDiffuse();

	value *= ht->Mat.GetReflect();

	for (int i = 1; i < maxDepth; i++)
	{
		current.Start = hit.nPos + hit.nNormal * 0.02f;
		current.Dir -= current.Dir * hit.nNormal * 2.0f;
		if (value < 0.05f)
		{
			break;
		}
		if (scene->CastRay(hit, current, colId))
		{
			ht = scene->getShapeByID(hit.id);
			res += hit.col * ht->Mat.GetDiffuse();
			value *= ht->Mat.GetReflect();
		}
		else
		{
			break;
		}
	}
	return res;
}