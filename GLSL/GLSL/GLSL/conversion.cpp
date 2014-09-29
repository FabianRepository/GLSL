#include "geometry.h"
Point<3> Vec3toPoint3(const vec3 in)
{
	return Point<3>(in[0], in[1], in[2]);
}
vec3 Point3toVec3(const Point<3> in)
{
	return vec3(in[0], in[1], in[2]);
}