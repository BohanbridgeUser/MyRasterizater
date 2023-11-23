#include "BlinnPhongShader.h"

namespace MyRasterizater {
BlinnPhongShader::BlinnPhongShader(unsigned int rId,
								   unsigned int rWidth,
								   unsigned int rHeight,
								   std::vector<float*>& rframe_buffer,
								   std::vector<float*>& rdepth_buffer)
	:FragmentShader(rId, rWidth, rHeight, rframe_buffer, rdepth_buffer)
{}
BlinnPhongShader::~BlinnPhongShader() {}
void BlinnPhongShader::Process(Triangle& triangle)
{
	using std::floor;
	using std::ceil;
	Vector3f blc, trc;
	Utility::FindEdgePs(triangle, blc, trc);
	int xmin = floor(blc[0]), xmax = ceil(trc[0]),
		ymin = floor(blc[1]), ymax = ceil(trc[1]);
	for (int y = ymin; y < ymax; ++y)
	{
		for (int x = xmin; x < xmax; ++x)
		{
			if (Utility::IsInTriangle(x + 0.5f, y + 0.5f,
									  triangle.m_f_posi[0][0], triangle.m_f_posi[0][1],
									  triangle.m_f_posi[1][0], triangle.m_f_posi[1][1],
									  triangle.m_f_posi[2][0], triangle.m_f_posi[2][1]))
			{
				Vector3f bary = Utility::BarycentricCoordinates(x + 0.5f, y + 0.5f, triangle);
				Vector3f w{ triangle.m_f_posi[0][3], triangle.m_f_posi[1][3], triangle.m_f_posi[2][3] };
				Vector3f z{ triangle.m_f_posi[0][2], triangle.m_f_posi[1][2], triangle.m_f_posi[2][2] };
				float zk = 1.0f / (bary[0] / w[0] + bary[1] / w[1] + bary[2] / w[2]);
				float z_depth = (bary[0] * z[0] / w[0] + bary[1] * z[1] / w[1] + bary[2] * z[2] / w[2]) * zk;
				if (z_depth - GetDepthBuffer(y,x) < 1e-20)
				{
					SetDepthBuffer(y, x, z_depth);
					Vector3f viewposi = Utility::CalBaryInterPosi(bary, triangle, w) * zk;
					Vector2f text =		Utility::CalBaryInterText(bary, triangle, w) * zk;
					Vector3f norm =		Utility::CalBaryInterNorm(bary, triangle, w) * zk;
					norm.normalize();
					Vector3f textvalue = GetTextureValue(text[0], text[1]);
					OBJ::Vector3f color = { 0.0f,0.0f,0.0f };
					auto& lights = GetLight();
					auto& camera = GetCamera();
					for (int li = 0; li < GetLight().size(); ++li)
					{
						float r = (lights[li].position - viewposi).norm();
						auto v = (camera - viewposi).normalized();
						auto l = (lights[li].position - viewposi).normalized();
						Vector3f halfv(((v + l) * 0.5).normalized());
						int p = 150;
						Vector3f Ls;
						Ls = (lights[li].intensity.array()) * (1.0f / (r * r)) * pow(std::max(0.00f, halfv.dot(norm)), p) * ks.array();
						Vector3f Lk;
						Lk = textvalue.array() / 255.0f * (lights[li].intensity.array()) * (1.0f / (r * r)) * std::max(0.00, (double)norm.dot(l));
						Vector3f La;
						La = lightambient.array() * ka.array();
						Vector3f L = Ls + Lk + La;
						color += L;
					}
					color *= 255.f;
					SetFrameBuffer(y, x, color);
				}
			}
		}
	}
}
void BlinnPhongShader::SetLightAmbient(Vector3f& ambient) { lightambient = ambient; }
void BlinnPhongShader::SetKa(Vector3f& rka) { ka = rka; }
void BlinnPhongShader::SetKs(Vector3f& rks) { ks = rks; }
}
