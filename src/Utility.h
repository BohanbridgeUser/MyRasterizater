#pragma once
#ifndef _UTILITY_H_
#define	_UTILITY_H_
#include "define.h"
namespace MyRasterizater {
class Utility
{
public:
	static Vector3f CalBaryInterPosi(Vector3f& Bary, Triangle& triangle, Vector3f& w);

	static Vector2f CalBaryInterText(Vector3f& Bary, Triangle& triangle, Vector3f& w);

	static Vector3f CalBaryInterNorm(Vector3f& Bary, Triangle& triangle, Vector3f& w);

	static void FindEdgePs(Triangle& rTriangle, Vector3f& blc, Vector3f& trc);

	static bool IsInTriangle(float px,  float py,
							 float t0x, float t0y,
							 float t1x, float t1y,
							 float t2x, float t2y);
	static Vector3f BarycentricCoordinates(float xp, float yp, Triangle& triangle);
};

}

#endif