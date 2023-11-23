#include "Utility.h"

namespace MyRasterizater {
void Utility::FindEdgePs(Triangle& rTriangle,
						 Vector3f& blc,
						 Vector3f& trc)
{
	using std::max;
	using std::min;
	float t0x = rTriangle.m_f_posi[0][0], t0y = rTriangle.m_f_posi[0][1], t0z = rTriangle.m_f_posi[0][2];
	float t1x = rTriangle.m_f_posi[1][0], t1y = rTriangle.m_f_posi[1][1], t1z = rTriangle.m_f_posi[1][2];
	float t2x = rTriangle.m_f_posi[2][0], t2y = rTriangle.m_f_posi[2][1], t2z = rTriangle.m_f_posi[2][2];
	blc[0] = min(t0x, min(t1x, t2x));
	blc[1] = min(t0y, min(t1y, t2y));
	blc[2] = min(t0z, min(t1z, t2z));
	trc[0] = max(t0x, max(t1x, t2x));
	trc[1] = max(t0y, max(t1y, t2y));
	trc[2] = max(t0z, max(t1z, t2z));
}

bool Utility::IsInTriangle(float px,  float py,
								  float t0x, float t0y,
								  float t1x, float t1y,
								  float t2x, float t2y)
{
	Vector2f vp(px, py);
	Vector2f vt0(t0x, t0y);
	Vector2f vt1(t1x, t1y);
	Vector2f vt2(t2x, t2y);

	Vector2f vpt0 = vp - vt0;
	Vector2f vt1t0 = vt1 - vt0;

	Vector2f vpt1 = vp - vt1;
	Vector2f vt2t1 = vt2 - vt1;

	Vector2f vpt2 = vp - vt2;
	Vector2f vt0t2 = vt0 - vt2;

	float cross1 = vt1t0[0] * vpt0[1] - vt1t0[1] * vpt0[0];
	float cross2 = vt2t1[0] * vpt1[1] - vt2t1[1] * vpt1[0];
	float cross3 = vt0t2[0] * vpt2[1] - vt0t2[1] * vpt2[0];

	if ((cross1 > 0.0f && cross2 > 0.0f && cross3 > 0.0f)
		|| (cross1 < 0 && cross2 < 0.0f && cross3 < 0.0f))
		return true;
	else return false;
}
Vector3f Utility::BarycentricCoordinates(float xp, float yp, Triangle& triangle)
{
	float x0 = triangle.m_b_posi[0][0], y0 = triangle.m_b_posi[0][1];
	float x1 = triangle.m_b_posi[1][0], y1 = triangle.m_b_posi[1][1];
	float x2 = triangle.m_b_posi[2][0], y2 = triangle.m_b_posi[2][1];
	float gama = ((xp - x0) * (y1 - y0) - (yp - y0) * (x1 - x0))
		/ ((x2 - x0) * (y1 - y0) - (y2 - y0) * (x1 - x0));
	float beta = ((xp - x0) * (y2 - y0) - (yp - y0) * (x2 - x0))
		/ ((x1 - x0) * (y2 - y0) - (y1 - y0) * (x2 - x0));
	float alpha = 1 - beta - gama;
	return { alpha, beta, gama };
}
Vector3f Utility::CalBaryInterPosi(Vector3f& Bary, Triangle& triangle, Vector3f& w)
{
	Vector3f Result;
	for (int i = 0; i < 3; ++i)
		Result[i] = Bary[0] * triangle.m_b_posi[0][i] / w[0]
		+ Bary[1] * triangle.m_b_posi[1][i] / w[1]
		+ Bary[2] * triangle.m_b_posi[2][i] / w[2];
	return Result;
}
Vector2f Utility::CalBaryInterText(Vector3f& Bary, Triangle& triangle, Vector3f& w)
{
	Vector2f Result;
	for (int i = 0; i < 2; ++i)
		Result[i] = Bary[0] * triangle.m_text[0][i] / w[0]
				  + Bary[1] * triangle.m_text[1][i] / w[1]
				  + Bary[2] * triangle.m_text[2][i] / w[2];
	return Result;
}
Vector3f Utility::CalBaryInterNorm(Vector3f& Bary, Triangle& triangle, Vector3f& w)
{
	Vector3f Result;
	for (int i = 0; i < 3; ++i)
		Result[i] = Bary[0] * triangle.m_norm[0][i] / w[0]
				  + Bary[1] * triangle.m_norm[1][i] / w[1]
				  + Bary[2] * triangle.m_norm[2][i] / w[2];
	return Result;
}
}

