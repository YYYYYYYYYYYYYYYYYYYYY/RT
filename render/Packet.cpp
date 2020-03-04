#include "Packet.h"

int Package::getid()
{
	return id;
}

Vector3 Package::getLocation()
{
	return newLocation;
}

Quaternion Package::getRotation()
{
	return newRotation;
}

Vector3 Package::getScale()
{
	return newScale;
}

char Package::getType()
{
	return type;
}