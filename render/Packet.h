#ifndef PACKET_H
#define PACKET_H

#include "Vector3.h"
#include "Quaternion.h"
#include "BasicShape.h"
//#pragma pack(push, 1)
// 0 - id of camera.
class Package
{
private:
	int id;
	Vector3 newLocation;
	Quaternion newRotation;
	Vector3 newScale;
	char type;
public:
	//type 0 - creation, type 1 - deletion, type 2 - change
	Package(char ntype, int nid, Vector3 newLoc, Quaternion newRot, Vector3 newSc)
	{
		id = nid;
		newLocation = newLoc;
		newRotation = newRot;
		newScale = newSc;
		type = ntype;
	}

	BasicShape * shape;

	char getType();

	int getid();

	Vector3 getLocation();

	Quaternion getRotation();

	Vector3 getScale();
};
//#pragma pack(pop)
#endif
