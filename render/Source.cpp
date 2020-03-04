#define _CRT_SECURE_NO_WARNINGS
#include "Sphere.h"
#include "BVHTree.h"
#include "Renderer.h"
#include <algorithm>

int main()
{
	CameraParam param = CameraParam(4, 90.0f);
	std::vector<BasicShape*> shapes;

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			for (int k = 0; k < 5; k++)
			{
				shapes.push_back(new Sphere(1, Vector3(float(i - 2), float(j - 2), float(k - 2)), Quaternion(), Vector3(0.5f, 0.5f, 0.5f)));
			}
		}
	}
	Renderer rd = Renderer(param);
	rd.initScene(shapes);
/*	shapes.push_back(new Sphere(0.4f, Vector3(0, 0, 1)));
	
	Package packet = Package(0, -1, Vector3(), Quaternion(), Vector3());
	packet.shape = shapes.back();
	rd.scene->acceptPacket(&packet, 1);

	packet = Package(1, 1, Vector3(), Quaternion(), Vector3());
	rd.scene->acceptPacket(&packet, 1);
	/*
	packet = Package(2, 7, Vector3(5, 5, 1.5f), Quaternion(Vector3(0, 1, 0), acosf(-1)/4.0f), Vector3(1, 1, 2));
	rd.scene->acceptPacket(&packet, 1);*/
	rd.cam.setLocation(Vector3(10, -15, 2));
	rd.cam.Rotate(Quaternion(rd.cam.Z, acosf(-1) / 12));
	freopen("out.ppm", "wb", stdout);
	uint8_t header[] = "P6 1920 1080 255\n";
	header[17] = 10;
	uint8_t *image = (uint8_t*)malloc(1920 * 1080 * 3 * sizeof(uint8_t));
	int32_t *ids = (int32_t*)malloc(1920 * 1080 * sizeof(int32_t));
	std::cerr << rd.scene->getSize() << std::endl;
	
	//rd.printScene();
	rd.CPURenderPPMImage(&image, &ids, 1920, 1080);
	rd.scene->printBenchmark();
	
	fwrite(header, sizeof(uint8_t), sizeof(header) - 1, stdout);
	fwrite(image, sizeof(uint8_t), 1920 * 1080 * 3, stdout);
	freopen("im.txt", "w", stdout);
	for (int i = 0; i < 1080; i++)
	{
		for (int j = 0; j < 1920; j++)
		{
			std::cout << ids[i * 1920 + j] << " ";
		}
		std::cout << "\n";
	}
	free(image);
	free(ids);
	return 0;
}