#include "Scene.h"

CUDA_HD void Scene::splistRemove(uint32_t id)
{
	uint32_t prv = shapeNodes[id].prev;
	uint32_t nxt = shapeNodes[id].next;
	shapeNodes[nxt].prev = prv;
	shapeNodes[prv].next = nxt;
}

CUDA_HD void Scene::splistAddAfter(uint32_t id, uint32_t toAdd)
{
	uint32_t nxt = shapeNodes[id].next;
	shapeNodes[toAdd].next = nxt;
	shapeNodes[toAdd].prev = id;
	shapeNodes[id].next = toAdd;
	shapeNodes[nxt].prev = toAdd;
}

CUDA_H int Scene::addElem(BasicShape* bSp)
{
	if (cnt < MAX_OBJECTS)
	{
		cnt++;
		uint32_t tmp = shapeNodes[FB].next;
		uint32_t id = shapeNodes[tmp].link;
		sp[shapeNodes[tmp].link] = bSp;
		bSp->setListNode(tmp);

		splistRemove(tmp);
		splistAddAfter(TB, tmp);
		return id;
	}
	return -1;
}

CUDA_H void Scene::removeElem(int id)
{
	sp[id]->kill();
	uint32_t tmp = sp[id]->GetListNode();
	splistRemove(tmp);
	splistAddAfter(FB, tmp);
}

CUDA_H void Scene::initScene(std::vector<BasicShape*> bsv)
{
	cnt = (uint16_t)bsv.size();
	for (uint32_t i = 0; i < cnt; i++)
	{
		sp[i] = bsv[i];
		sp[i]->setListNode(i);
		shapeNodes[i].link = i;
		splistAddAfter(TB, i);
	}
	for (int i = cnt; i < MAX_OBJECTS; i++)
	{
		shapeNodes[i].link = i;
		splistAddAfter(FB, i);
	}
	rebuild();
}

CUDA_H size_t Scene::getSize()
{
	return sizeof(*this);
}

CUDA_HD bool Scene::CastRay(HitResult & res, const Ray& ray, int color, int32_t start)
{
	bool found = false;
	uint32_t id = shapeNodes[TB].next;
	res.dist = 1e9;
	while (id < MAX_OBJECTS)
	{
		HitResult cur;
		iC++;
		if (sp[shapeNodes[id].link]->hit(ray, cur, color))
		{
			if (cur.dist < res.dist)
			{
				found = true;
				res = cur;
				res.id = shapeNodes[id].link;
			}
		}
		id = shapeNodes[id].next;
	}
	return found;
}

CUDA_D void Scene::clear()
{
	
}

CUDA_H  void Scene::rebuild()
{
	return;
}

CUDA_H void Scene::acceptPacket(Package* packet, bool fastUpdate)
{
	if (packet->getType() == 0)
	{
		addElem(packet->shape);
	}
	else if (packet->getType() == 1)
	{
		removeElem(packet->getid());
	}
	else
	{
		int id = packet->getid();
		sp[id]->setLocation(packet->getLocation());
		sp[id]->Rotate(packet->getRotation());
		sp[id]->setScale(packet->getScale());
	}
}

CUDA_H void Scene::getSceneObjects(std::vector<BasicShape*>& objs)
{
	objs.clear();
	uint32_t id = shapeNodes[TB].next;
	while (id < MAX_OBJECTS)
	{
		objs.push_back(sp[shapeNodes[id].link]);
		id = shapeNodes[id].next;
	}
}

CUDA_H void Scene::printBenchmark()
{
	std::cerr << "Intersections count: " << iC << std::endl;
}


#ifdef CUDA
CUDA_H void Scene::moveToCUDA(Scene** gScene)
{

}
#endif