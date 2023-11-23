#include "rasterizer.h"
#include <cmath>

namespace MyRasterizater {
Rasterizter::Rasterizter(int width, int height)
	:m_width(width), m_height(height),
	m_vbo(VertexBuffer()),
	m_frame_buffer(NULL),
	m_depth_buffer(NULL)
{

}
Rasterizter::~Rasterizter()
{
	if (m_frame_buffer != NULL)
		free(m_frame_buffer);
	if (m_depth_buffer != NULL)
		free(m_depth_buffer);
	for (auto frame : m_frames_buffer)
		delete frame;
	for (auto depth : m_depths_buffer)
		delete depth;
}
/**
* @name Access
*/
float* Rasterizter::GetFrameBuffer() const { return m_frame_buffer; }
VertexBuffer& Rasterizter::GetVBO() { return m_vbo; }
std::vector<OBJ::Vector3f>& Rasterizter::GetIBO() { return m_ibo; }
float* Rasterizter::GetPosition(unsigned int index)
{
	return m_vbo.GetPosition(index);
}
float* Rasterizter::GetTexture(unsigned int index)
{
	return m_vbo.GetTexture(index);
}
float* Rasterizter::GetNormal(unsigned int index)
{
	return m_vbo.GetNormal(index);
}
Texture& Rasterizter::GetTextureBuffer(unsigned int index)
{
	return m_textures_buffer[index];
}
OBJ::Vector3f Rasterizter::GetTextureValue(float u, float v)
{
	return m_textures_buffer[0].GetTextrue(u, v);
}
/**
* @name Operation
*/
void Rasterizter::SetVBO(OBJ::OBJ_Importer& model)
{
	m_vbo.ImportData(model);
}
void Rasterizter::SetIBO(OBJ::OBJ_Importer& model)
{
	auto& faces = model.GetFaces();
	for (int i = 0; i < faces.size() * 3; i += 3)
		m_ibo.push_back(OBJ::Vector3f(i, i + 1, i + 2));
}
void Rasterizter::PushLight(Light& light) { m_light.push_back(light); }
void Rasterizter::SetCamera(OBJ::Vector3f const& rCamera)
{
	m_camera = rCamera;
}
void Rasterizter::SetModelm(glm::mat4 const& mat)
{
	m_modelm = mat;
}
void Rasterizter::SetViewm(glm::mat4 const& mat)
{
	m_viewm = mat;
}
void Rasterizter::SetProjm(glm::mat4 const& mat)
{
	m_proj = mat;
}
void Rasterizter::InputTexture(std::string const& filepath)
{
	m_textures_buffer.emplace_back(filepath);
}

glm::mat4 Rasterizter::translate(glm::mat4 mat, glm::vec3 translater)
{
	mat[3] = mat[0] * translater[0] + mat[1] * translater[1] + mat[2] * translater[2] + mat[3];
	return mat;
}
glm::mat4 Rasterizter::scale(glm::mat4 mat, glm::vec3 scaler)
{
	mat[0] = mat[0] * scaler[0];
	mat[1] = mat[1] * scaler[1];
	mat[2] = mat[2] * scaler[2];
	return mat;
}
glm::mat4 Rasterizter::rotation(glm::mat4 mat, float angle, glm::vec3 axis)
{
	glm::mat3 I = glm::mat4(1.0f);
	glm::mat3 nnt(0.0f);
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			nnt[j][i] = axis[i] * axis[j];
	glm::mat3 crossmatrix;
	crossmatrix[0][0] = 0.0f; crossmatrix[1][0] = -axis[2]; crossmatrix[2][0] = axis[1];
	crossmatrix[0][1] = axis[2]; crossmatrix[1][1] = 0.00f; crossmatrix[2][1] = -axis[0];
	crossmatrix[0][2] = -axis[1]; crossmatrix[1][2] = axis[0]; crossmatrix[2][2] = 0.00f;
	glm::mat4 result(std::cos(angle) * I + (1 - std::cos(angle)) * nnt + std::sin(angle) * crossmatrix);
	result[3][3] = 1.00f;
	return result;
}
glm::mat4 Rasterizter::perspective(float fov, float aspect, float zNear, float zFar)
{
	glm::mat4 result(0.0f);
	float tanHalffov = std::tan(fov / 2.00f);
	result[0][0] = 1.00f / (tanHalffov * aspect);
	result[1][1] = 1.00f / tanHalffov;
	result[2][2] = -(zFar + zNear) / (zFar - zNear);
	result[3][2] = -2.f * (zFar * zNear) / (zFar - zNear);
	result[2][3] = -1.00f;
	return result;
}
glm::mat4 Rasterizter::ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	glm::mat4 result;
	result[0][0] = 2.00f / (right - left);
	result[3][0] = -(right + left) / (right - left);
	result[1][1] = 2.00f / (top - bottom);
	result[3][1] = -(top + bottom) / (top - bottom);
	result[2][2] = 2.00f / (zFar - zNear);
	result[3][2] = -(zFar + zNear) / (zFar - zNear);
	result[3][3] = 1.00f;
	return result;
}

void Rasterizter::FindEdgePs(float* v_data,
	int v0, int v1, int v2,
	glm::vec3& blc, glm::vec3& trc)
{
	using std::min;
	using std::max;
	unsigned int stride = GetVBO().GetStride();
	float t0x = v_data[v0 * stride], t0y = v_data[v0 * stride + 1], t0z = v_data[v0 * stride + 2];
	float t1x = v_data[v1 * stride], t1y = v_data[v1 * stride + 1], t1z = v_data[v1 * stride + 2];
	float t2x = v_data[v2 * stride], t2y = v_data[v2 * stride + 1], t2z = v_data[v2 * stride + 2];
	blc[0] = min(t0x, min(t1x, t2x));
	blc[1] = min(t0y, min(t1y, t2y));
	blc[2] = min(t0z, min(t1z, t2z));
	trc[0] = max(t0x, max(t1x, t2x));
	trc[1] = max(t0y, max(t1y, t2y));
	trc[2] = max(t0z, max(t1z, t2z));
}
void Rasterizter::FindEdgePs(std::vector<glm::vec4> mvpv,
	glm::vec3& blc,
	glm::vec3& trc)
{
	using std::max;
	using std::min;
	float t0x = mvpv[0][0], t0y = mvpv[0][1], t0z = mvpv[0][2];
	float t1x = mvpv[1][0], t1y = mvpv[1][1], t1z = mvpv[1][2];
	float t2x = mvpv[2][0], t2y = mvpv[2][1], t2z = mvpv[2][2];
	blc[0] = min(t0x, min(t1x, t2x));
	blc[1] = min(t0y, min(t1y, t2y));
	blc[2] = min(t0z, min(t1z, t2z));
	trc[0] = max(t0x, max(t1x, t2x));
	trc[1] = max(t0y, max(t1y, t2y));
	trc[2] = max(t0z, max(t1z, t2z));
}

void Rasterizter::Draw(unsigned int index)
{
	unsigned int verticescnt = m_vbo.GetCount();
	unsigned int stride = m_vbo.GetStride();
	unsigned int trianglescnt = m_ibo.size();
	m_frames_buffer[index] = new float[m_width * m_height * 3];
	m_depths_buffer[index] = new float[m_width * m_height];
	for (unsigned int i = 0; i < m_width * m_height; ++i)
		*(m_depth_buffer + i) = 2e31f;
	for (unsigned int tri = 0; tri < trianglescnt; ++tri)
	{
		Triangle triangle;
		OBJ::Vector3f voftri = m_ibo[tri];
		float* v[3], * t[3], * n[3];
		for (int i = 0; i < 3; ++i)
		{
			v[i] = GetPosition(voftri[i]);
			t[i] = GetTexture(voftri[i]);
			n[i] = GetNormal(voftri[i]);
			triangle.m_b_posi.emplace_back(v[i][0], v[i][1], v[i][2], 1.00f);
			triangle.m_f_posi.emplace_back(v[i][0], v[i][1], v[i][2], 1.00f);
			triangle.m_text.emplace_back(t[i][0], t[i][1], t[i][2]);
			triangle.m_norm.emplace_back(n[i][0], n[i][1], n[i][2]);
		}
		mp_vertexshader->Process(triangle);
		mp_fragmentshader->Process(triangle);
	}
}

void Rasterizter::Draw()
{
	unsigned int verticescnt = m_vbo.GetCount();
	unsigned int stride = m_vbo.GetStride();
	unsigned int trianglescnt = m_ibo.size();
	m_frame_buffer = (float*)malloc(sizeof(float) * m_width * m_height * 3);
	memset(m_frame_buffer, 0, sizeof(float) * m_width * m_height * 3);
	m_depth_buffer = (float*)malloc(sizeof(float) * m_width * m_height);
	for (int i = 0; i < m_width * m_height; ++i)
		*(m_depth_buffer + i) = 2e31;
	std::cout << "Vertices count : " << verticescnt << std::endl;
	std::cout << "Triangles count : " << trianglescnt << std::endl;
	int cnt = 0;

	for (int tri = 0; tri < trianglescnt; ++tri)
		VerticesShader(tri);
}
void Rasterizter::VerticesShader(unsigned int index)
{
	float f1 = (50 - 0.1) / 2.0;
	float f2 = (50 + 0.1) / 2.0;
	glm::mat4 viewportm = viewport(m_width, m_height);
	OBJ::Vector3f voftri = m_ibo[index];
	float* v[3], * t[3], * n[3];
	for (int i = 0; i < 3; ++i)
	{
		v[i] = GetPosition(voftri[i]);
		t[i] = GetTexture(voftri[i]);
		n[i] = GetNormal(voftri[i]);
	}
	std::vector<glm::vec2> pt(3);
	std::vector<glm::vec4> pn(3);
	std::vector<glm::vec4> pv(3);
	std::vector<glm::vec4> mvppv(3);
	m_proj *= -1;
	for (int i = 0; i < 3; ++i)
	{
		pv[i] = glm::vec4(v[i][0], v[i][1], v[i][2], 1.00f);
		mvppv[i] = m_proj * m_viewm * m_modelm * pv[i];

		for (int j = 0; j < 3; ++j)
			mvppv[i][j] = mvppv[i][j] / mvppv[i][3];
		mvppv[i][0] = 0.5 * m_width * (mvppv[i][0] + 1.0);
		mvppv[i][1] = 0.5 * m_height * (mvppv[i][1] + 1.0);
		mvppv[i][2] = mvppv[i][2] * f1 + f2;
		pv[i] = m_viewm * m_modelm * pv[i];
		pt[i] = glm::vec2(t[i][0], t[i][1]);
		pn[i] = glm::vec4(n[i][0], n[i][1], n[i][2], 0.0f);
		pn[i] = glm::transpose(glm::inverse(m_viewm * m_modelm)) * pn[i];
	}
	// TODO::Clipper
	//
	BlinnPhongFragmentShader(mvppv, pv, pt, pn);
}
void Rasterizter::BlinnPhongFragmentShader(std::vector<glm::vec4>& mvpv,
	std::vector<glm::vec4>& mvv,
	std::vector<glm::vec2>& pt,
	std::vector<glm::vec4>& pn)
{
	OBJ::Vector3f lightambient(10.0f, 10.0f, 10.0f);
	OBJ::Vector3f ka(0.005f, 0.005f, 0.005f);
	OBJ::Vector3f ks(0.7937f, 0.7937f, 0.7937f);
	using std::floor;
	using std::ceil;
	glm::vec3 blc, trc;
	FindEdgePs(mvpv, blc, trc);
	int xmin = floor(blc[0]), xmax = ceil(trc[0]),
		ymin = floor(blc[1]), ymax = ceil(trc[1]);
	for (int j = ymin; j < ymax; ++j)
	{
		for (int k = xmin; k < xmax; ++k)
		{
			if (IsInTriangle(k + 0.5f, j + 0.5f,
				mvpv[0][0], mvpv[0][1],
				mvpv[1][0], mvpv[1][1],
				mvpv[2][0], mvpv[2][1]))
			{
				OBJ::Vector3f bary = BarycentricCoordinates(k + 0.5f, j + 0.5f,
					mvpv[0][0], mvpv[0][1],
					mvpv[1][0], mvpv[1][1],
					mvpv[2][0], mvpv[2][1]);

				float zk = 1.0f / (bary[0] / mvpv[0][3] + bary[1] / mvpv[1][3] + bary[2] / mvpv[2][3]);
				float z_depth = (bary[0] * mvpv[0][2] / mvpv[0][3] +
					bary[1] * mvpv[1][2] / mvpv[1][3] +
					bary[2] * mvpv[2][2] / mvpv[2][3]) * zk;
				if (z_depth - (*(m_depth_buffer + (m_height - 1 - j) * m_width + k)) < 1e-20)
				{
					auto viewposi = CalBaryInterGLM(bary, mvv[0], mvv[1], mvv[2], mvpv[0][3], mvpv[1][3], mvpv[2][3]) * zk;
					auto text = CalBaryInterGLM(bary, pt[0], pt[1], pt[2], mvpv[0][3], mvpv[1][3], mvpv[2][3]) * zk;
					auto norm = CalBaryInterGLM(bary, pn[0], pn[1], pn[2], mvpv[0][3], mvpv[1][3], mvpv[2][3]) * zk;
					OBJ::Vector3f norm2(norm[0], norm[1], norm[2]);
					norm2 = norm2.normalized();
					OBJ::Vector3f viewposi2(viewposi[0], viewposi[1], viewposi[2]);
					OBJ::Vector3f textvalue = GetTextureValue(text[0], text[1]);
					OBJ::Vector3f color = { 0.0f,0.0f,0.0f };
					for (int li = 0; li < m_light.size(); ++li)
					{
						float r = (m_light[li].position - viewposi2).norm();
						auto v = (m_camera - viewposi2).normalized();
						auto l = (m_light[li].position - viewposi2).normalized();
						OBJ::Vector3f halfv(((v + l) * 0.5).normalized());
						int p = 150;
						OBJ::Vector3f Ls;
						Ls = (m_light[li].intensity.array()) * (1.0f / (r * r)) * pow(std::max(0.00f, halfv.dot(norm2)), p) * ks.array();
						OBJ::Vector3f Lk;
						Lk = textvalue.array() / 255.0f * (m_light[li].intensity.array()) * (1.0f / (r * r)) * std::max(0.00, (double)norm2.dot(l));
						OBJ::Vector3f La;
						La = lightambient.array() * ka.array();
						OBJ::Vector3f L = Ls + Lk + La;
						color += L;
					}
					*(m_depth_buffer + (m_height - 1 - j) * m_width + k) = z_depth;
					*(m_frame_buffer + (m_height - 1 - j) * m_width * 3 + 3 * k) = color[0] * 255.f;
					*(m_frame_buffer + (m_height - 1 - j) * m_width * 3 + 3 * k + 1) = color[1] * 255.f;
					*(m_frame_buffer + (m_height - 1 - j) * m_width * 3 + 3 * k + 2) = color[2] * 255.f;
				}
			}
		}
	}
}


glm::mat4 Rasterizter::viewport(float width, float height)
{
	glm::mat4 result;
	result[0][0] = width / 2.00f;
	result[1][1] = height / 2.00f;
	result[3][0] = width / 2.00f;
	result[3][1] = height / 2.00f;
	result[2][2] = 1.00f;
	result[3][3] = 1.00f;
	return result;
}
inline bool Rasterizter::IsInTriangle(float px, float py,
	float t0x, float t0y,
	float t1x, float t1y,
	float t2x, float t2y)
{
	glm::vec2 vp(px, py);
	glm::vec2 vt0(t0x, t0y);
	glm::vec2 vt1(t1x, t1y);
	glm::vec2 vt2(t2x, t2y);

	glm::vec2 vpt0 = vp - vt0;
	glm::vec2 vt1t0 = vt1 - vt0;

	glm::vec2 vpt1 = vp - vt1;
	glm::vec2 vt2t1 = vt2 - vt1;

	glm::vec2 vpt2 = vp - vt2;
	glm::vec2 vt0t2 = vt0 - vt2;

	float cross1 = vt1t0[0] * vpt0[1] - vt1t0[1] * vpt0[0];
	float cross2 = vt2t1[0] * vpt1[1] - vt2t1[1] * vpt1[0];
	float cross3 = vt0t2[0] * vpt2[1] - vt0t2[1] * vpt2[0];

	if ((cross1 > 0.0f && cross2 > 0.0f && cross3 > 0.0f)
		|| (cross1 < 0 && cross2 < 0.0f && cross3 < 0.0f))
		return true;
	else return false;
}
OBJ::Vector3f Rasterizter::BarycentricCoordinates(float xp, float yp,
	float x0, float y0,
	float x1, float y1,
	float x2, float y2)
{
	float gama = ((xp - x0) * (y1 - y0) - (yp - y0) * (x1 - x0))
		/ ((x2 - x0) * (y1 - y0) - (y2 - y0) * (x1 - x0));
	float beta = ((xp - x0) * (y2 - y0) - (yp - y0) * (x2 - x0))
		/ ((x1 - x0) * (y2 - y0) - (y1 - y0) * (x2 - x0));
	float alpha = 1 - beta - gama;
	return OBJ::Vector3f(alpha, beta, gama);
}
OBJ::Vector3f Rasterizter::BarycentricCoordinates2(double xp, double yp,
	double x0, double y0,
	double x1, double y1,
	double x2, double y2)
{
	double c1 = (xp * (y1 - y2) + (x2 - x1) * yp + x1 * y2 - x2 * y1)
		/ (x0 * (y1 - y2) + (x2 - x1) * y0 + x1 * y2 - x2 * y1);
	double c2 = (xp * (y2 - y0) + (x0 - x2) * yp + x2 * y0 - x0 * y2)
		/ (x1 * (y2 - y0) + (x0 - x2) * y1 + x2 * y0 - x0 * y2);
	double c3 = (xp * (y0 - y1) + (x1 - x0) * yp + x0 * y1 - x1 * y0)
		/ (x2 * (y0 - y1) + (x1 - x0) * y2 + x0 * y1 - x1 * y0);
	return OBJ::Vector3f(c1, c2, c3);
}

}
