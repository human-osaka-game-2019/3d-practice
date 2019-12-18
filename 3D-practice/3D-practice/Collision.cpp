#include "Collision.h"



FLOAT AbsVector(D3DXVECTOR3 vector)
{
	FLOAT X = vector.x * vector.x;
	FLOAT Y = vector.y * vector.y;
	FLOAT Z = vector.z * vector.z;
	FLOAT XYZ = X + Y + Z;
	FLOAT vector_size = sqrt(XYZ);
	return vector_size;
}




bool isCollision(THING thing1, THING thing2)
{
	if (thing1.Sphere.fRadius + thing2.Sphere.fRadius > AbsVector(thing1.Sphere.vecCenter - thing2.Sphere.vecCenter)) return true;
	return false;
}
