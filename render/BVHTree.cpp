#include "BVHTree.h"

CUDA_HD void BVHTree::fllistRemove(int32_t id)
{
	int32_t prv = freelist[id].prev;
	int32_t nxt = freelist[id].next;
	freelist[nxt].prev = prv;
	freelist[prv].next = nxt;
}

CUDA_HD void BVHTree::fllistAddAfter(uint32_t id, uint32_t toAdd)
{
	uint32_t nxt = freelist[id].next;
	freelist[toAdd].next = nxt;
	freelist[toAdd].prev = id;
	freelist[id].next = toAdd;
	freelist[nxt].prev = toAdd;
}

CUDA_H void BVHTree::add(uint16_t link)
{
	if (root != -1)
	{
		insert(link, root, -1);
	}
	else
	{
		int32_t tmp = freelist[LB].next;
		fllistRemove(tmp);
		root = tmp;
		nodes[tmp].bb = sp[link]->bb;
		nodes[tmp].cnt = 1;
		nodes[tmp].depth = 1;
		nodes[tmp].obj = link;
	}
}

CUDA_H int32_t BVHTree::insert(uint16_t link, int32_t id, int32_t rlink)
{
	if (nodes[id].leaf())
	{
		uint32_t id1 = freelist[LB].next;
		fllistRemove(id1);
		uint32_t id2 = freelist[LB].next;
		fllistRemove(id2);

		sp[link]->link1 = id2;
		nodes[id2].bb = sp[link]->bb;
		nodes[id2].cnt = 1;
		nodes[id2].depth = 1;
		nodes[id2].link = rlink;
		nodes[id2].obj = link;
		nodes[id2].parent = id1;

		nodes[id1].left = id;
		nodes[id1].right = id2;

		nodes[id1].bb = nodes[id].bb;
		nodes[id1].bb.join(nodes[id2].bb);
		nodes[id1].depth = 2;
		nodes[id1].cnt = 2;
		nodes[id1].link = nodes[id].link;

		nodes[id].link = id2;

		
		if (nodes[id].parent == -1) // if current node is root
		{	
			root = id1;
		}
		else
		{
			uint32_t id3 = nodes[id].parent;
			if (id == nodes[id3].left)
			{
				nodes[id3].left = id1;
			}
			else
			{
				nodes[id3].right = id1;
			}
			nodes[id1].parent = id3;
		}
		nodes[id].parent = id1;
		return id1;
	}
	else
	{
		nodes[id].cnt++;
		nodes[id].bb.join(sp[link]->bb);
		int32_t l = nodes[id].left;
		int32_t r = nodes[id].right;
		float sahl1 = nodes[l].bb.surfaceArea() * nodes[l].cnt;
		float sahr1 = nodes[r].bb.surfaceArea() * nodes[r].cnt;

		BBox bl = nodes[l].bb;
		bl.join(sp[link]->bb);
		BBox br = nodes[r].bb;
		br.join(sp[link]->bb);
		
		float sahl2 = bl.surfaceArea() * (nodes[l].cnt + 1);
		float sahr2 = br.surfaceArea() * (nodes[r].cnt + 1);

		if (sahl2 + sahr1 <= sahr2 + sahl1) // going left
		{
			int32_t next = insert(link, l, r);
			nodes[id].depth = 1 + max(nodes[r].depth, nodes[next].depth);
			if (nodes[next].depth - nodes[r].depth == 2)
			{
				id = rotateRight(id);
			}
		}
		else // going right
		{
			int32_t next = insert(link, r, rlink);
			nodes[id].right = next;
			nodes[id].depth = 1 + max(nodes[l].depth, nodes[next].depth);
			if (nodes[next].depth - nodes[l].depth == 2)
			{
				id = rotateLeft(id);
			}
		}
		if (nodes[id].parent == -1) root = id;
		return id;
	}
}

CUDA_H float BVHTree::nodesJoin(BVHnode & nd1, BVHnode & nd2)
{
	BBox bb = nd1.bb;
	bb.join(nd2.bb);
	return bb.surfaceArea() * (nd1.cnt + nd2.cnt);
}

CUDA_H int32_t BVHTree::rotateRight(int32_t id)
{
	int32_t par = nodes[id].parent;
	int32_t l = nodes[id].left;
	int32_t r = nodes[id].right;
	int32_t ll = nodes[l].left;
	int32_t lr = nodes[l].right;

	float v1 = nodesJoin(nodes[ll], nodes[r]);
	float v2 = nodesJoin(nodes[lr], nodes[r]);

	float bvl = nodes[ll].SAH();
	float bvr = nodes[lr].SAH();

	if (bvr + v1 < bvl + v2)
	{
		nodes[ll].parent = id;
		nodes[id].left = ll;

		nodes[id].bb = nodes[ll].bb;
		nodes[id].bb.join(nodes[r].bb);
		nodes[id].cnt = nodes[ll].cnt + nodes[r].cnt;
		nodes[id].depth = 1 + max(nodes[ll].depth, nodes[r].depth);
		
		nodes[l].bb = nodes[lr].bb;
		nodes[l].cnt = nodes[lr].cnt;
		nodes[l].depth = nodes[lr].depth;
		nodes[l].left = id;
	}
	else
	{
		nodes[lr].parent = id;
		nodes[id].left = lr;

		nodes[id].bb = nodes[lr].bb;
		nodes[id].bb.join(nodes[r].bb);
		nodes[id].cnt = nodes[lr].cnt + nodes[r].cnt;
		nodes[id].depth = 1 + max(nodes[lr].depth, nodes[r].depth);

		nodes[l].bb = nodes[ll].bb;
		nodes[l].cnt = nodes[ll].cnt;
		nodes[l].depth = nodes[ll].depth;
		nodes[l].right = id;
	}
	nodes[id].parent = l;
	nodes[l].bb.join(nodes[id].bb);
	nodes[l].cnt += nodes[id].cnt;
	nodes[l].depth = 1 + max(nodes[id].depth, nodes[l].depth);
	nodes[l].parent = par;
	if (par != -1)
	{
		if (nodes[par].left == id) nodes[par].left = l;
		else nodes[par].right = l;
	}
	return l;
}

CUDA_H int32_t BVHTree::rotateLeft(int32_t id)
{
	int32_t par = nodes[id].parent;
	int32_t l = nodes[id].left;
	int32_t r = nodes[id].right;
	int32_t rl = nodes[r].left;
	int32_t rr = nodes[r].right;

	float v1 = nodesJoin(nodes[rl], nodes[l]);
	float v2 = nodesJoin(nodes[rr], nodes[l]);

	float bvl = nodes[rl].SAH();
	float bvr = nodes[rr].SAH();

	if (bvr + v1 < bvl + v2)
	{
		nodes[rl].parent = id;
		nodes[id].right = rl;

		nodes[id].bb = nodes[rl].bb;
		nodes[id].bb.join(nodes[l].bb);
		nodes[id].cnt = nodes[rl].cnt + nodes[l].cnt;
		nodes[id].depth = 1 + max(nodes[rl].depth, nodes[l].depth);

		nodes[r].bb = nodes[rr].bb;
		nodes[r].cnt = nodes[rr].cnt;
		nodes[r].depth = nodes[rr].depth;
		nodes[r].left = id;
	}
	else
	{
		nodes[rr].parent = id;
		nodes[id].right = rr;

		nodes[id].bb = nodes[rr].bb;
		nodes[id].bb.join(nodes[l].bb);
		nodes[id].cnt = nodes[rr].cnt + nodes[l].cnt;
		nodes[id].depth = 1 + max(nodes[rr].depth, nodes[l].depth);

		nodes[r].bb = nodes[rl].bb;
		nodes[r].cnt = nodes[rl].cnt;
		nodes[r].depth = nodes[rl].depth;
		nodes[r].right = id;
	}
	nodes[id].parent = r;
	nodes[r].bb.join(nodes[id].bb);
	nodes[r].cnt += nodes[id].cnt;
	nodes[r].depth = 1 + max(nodes[id].depth, nodes[r].depth);
	nodes[r].parent = par;
	if (par != -1)
	{
		if (nodes[par].left == id) nodes[par].left = r;
		else nodes[par].right = r;
	}
	return r;
}

CUDA_H void BVHTree::remove(int32_t id)
{
	if (root == id)
	{
		root = -1;
		fllistRemove(id);
	}
	else
	{
		int32_t par = nodes[id].parent;
		if (par == root)
		{
			if (nodes[par].left == id) root = nodes[par].right;
			else root = nodes[par].left;
			fllistRemove(id);
		}
		else
		{
			int32_t ppar = nodes[par].parent;
			if (nodes[par].left == id)
			{
				if (nodes[ppar].left == par)
				{
					nodes[ppar].left = nodes[par].right;
				}
				else
				{
					nodes[ppar].right = nodes[par].right;
				}
			}
			else
			{
				if (nodes[ppar].left == par)
				{
					nodes[ppar].left = nodes[par].left;
				}
				else
				{
					nodes[ppar].right = nodes[par].left;
				}
			}
			fllistRemove(id);
			while (ppar != -1)
			{
				int32_t l = nodes[ppar].left;
				int32_t r = nodes[ppar].right;

				nodes[ppar].cnt = nodes[l].cnt + nodes[r].cnt;

				nodes[ppar].bb = nodes[r].bb;
				nodes[ppar].bb.join(nodes[l].bb);


				if (nodes[l].depth - nodes[r].depth == 2)
				{
					ppar = rotateRight(ppar);
				}
				else if (nodes[r].depth - nodes[l].depth == 2)
				{
					ppar = rotateLeft(ppar);
				}
				ppar = nodes[ppar].parent;
			}
		}
	}
}

CUDA_H bool BVHTree::CPU_traverse(HitResult& res, const Ray& r, int color, int cur)
{
	bool hhit = false;
	if (nodes[root].leaf())
	{
		return sp[nodes[root].obj]->hit(r, res, color);
	}
	else
	{
		int32_t cur = root;
		int32_t prev = -1;
		while (1)
		{
			if (cur == root && prev == nodes[root].right)
			{
				break;
			}
			if (nodes[cur].leaf())
			{
				HitResult hitres; 
				if (sp[nodes[cur].obj]->hit(r, hitres, color))
				{
					hhit = 1;
					if (hitres.dist < res.dist) 
					{
						res = hitres;
						res.id = nodes[cur].obj;
					}
				}
				prev = cur;
				cur = nodes[cur].parent;
			}
			else if (prev == nodes[cur].right) // going up 
			{
				prev = cur;
				cur = nodes[cur].parent;
			}
			else
			{
				if (prev == nodes[cur].parent) // going left
				{
					int32_t l = nodes[cur].left;
					float near, far;
					if (nodes[l].bb.checkIntersection(r, near, far))
					{
						prev = cur;
						cur = l;
					}
					else
					{
						prev = l;
					}
				}
				else // going right
				{
					int32_t rt = nodes[cur].right;
					float near, far;
					if (nodes[rt].bb.checkIntersection(r, near, far))
					{
						prev = cur;
						cur = rt;
					}
					else
					{
						prev = rt;
					}
				}
			}
		}
	}
	return hhit;
	
	if (nodes[cur].leaf())
	{
		HitResult curRes;
		iC++;
		if (sp[nodes[cur].obj]->hit(r, curRes, color))
		{
			curRes.id = nodes[cur].obj;
			if (curRes.dist < res.dist)
			{
				res = curRes;
				return true;
			}
		}
		return false;
	}
	else
	{
		int32_t ld = nodes[cur].left;
		int32_t rd = nodes[cur].right;

		//iC++;
		float tn;
		float s;
		//iC++;
		bool bres = false;
		if (nodes[ld].bb.checkIntersection(r,tn, s) && tn < res.dist)
		{
			bres |= CPU_traverse(res, r, color, ld);
		}
		if (nodes[rd].bb.checkIntersection(r, tn, s) && tn < res.dist)
		{
			bres |= CPU_traverse(res, r, color, rd);
		}
		return bres;
	}
}

CUDA_HD bool BVHTree::CastRay(HitResult & res, const Ray & ray, int color, int32_t start)
{
	if (start == -1) start = root;

	return CPU_traverse(res, ray, color, start);
}

CUDA_H void BVHTree::rebuild()
{
	uint32_t id = shapeNodes[TB].next;
	while (id < MAX_OBJECTS)
	{
		add(shapeNodes[id].link);
		id = shapeNodes[id].next;
	}
	std::cerr << "Built!" << std::endl;
}

CUDA_H void BVHTree::acceptPacket(Package* package, bool fastUpdate)
{
	if (package->getType() == 0)
	{
		int id = addElem(package->shape);
		if (id != -1)
			add(id & ((1 << 16) - 1));
	}
	else if (package->getType() == 1)
	{
		removeElem(package->getid());
		remove(sp[package->getid()]->link1);
	}
	else
	{
		int id = package->getid();
		sp[id]->setLocation(package->getLocation());
		sp[id]->Rotate(package->getRotation());
		sp[id]->setScale(package->getScale());
	}
}

CUDA_H void BVHTree::print()
{
	int cntV = dfs(root);
	std::cerr << "Number of leaves: " << cntV << std::endl;
}

int BVHTree::dfs(int v)
{
	std::cerr << "Node #" << v << ", parent " << nodes[v].parent << "\nleft child " << nodes[v].left << ", right child " << nodes[v].right << "\ncnr: " << nodes[v].cnt << std::endl;
	int a = 0;
	if (nodes[v].leaf()) return 1;
	a += dfs(nodes[v].left);
	a += dfs(nodes[v].right);
	return a;
}
