#pragma once
#ifndef _RASTERIZER_H_
#define _RASTERIZER_H_

#include <vector>
#include <cstring>
#include <fstream>
#include <cmath>
#include <functional>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "OBJ_Import.h"
#include "VertexBuffer.h"
#include "Mywindow.h"
#include "Texture.h"
#include "../shader/VertexShader.h"
#include "../shader/BlinnPhongShader.h"

namespace MyRasterizater {
class Rasterizter {
public:
	/**
	* @name Life Circle
	*/
	Rasterizter(int width, int height);
	~Rasterizter();

	/**
	* @name Access
	*/
	float* GetFrameBuffer() const;
	VertexBuffer& GetVBO();
	std::vector<OBJ::Vector3f>& GetIBO();
	float* GetPosition(unsigned int index);
	float* GetTexture(unsigned int index);
	float* GetNormal(unsigned int index);
	Texture& GetTextureBuffer(unsigned int index);
	OBJ::Vector3f GetTextureValue(float u, float v);
	/**
	* @name Operation
	*/
	void SetVBO(OBJ::OBJ_Importer& model);
	void SetIBO(OBJ::OBJ_Importer& model);
	void SetModelm(glm::mat4 const& mat);
	void SetViewm(glm::mat4 const& mat);
	void SetProjm(glm::mat4 const& mat);
	void PushLight(Light& light);
	void SetCamera(OBJ::Vector3f const& rCamera);
	void InputTexture(std::string const& filepath);
	/* translate scale rotation perspective ortho viewport transformation */
	glm::mat4 translate(glm::mat4 mat, glm::vec3 translater);
	glm::mat4 scale(glm::mat4 mat, glm::vec3 scaler);
	glm::mat4 rotation(glm::mat4 mat, float angle, glm::vec3 axis);
	glm::mat4 perspective(float fov, float aspect, float zNear, float zFar);
	glm::mat4 ortho(float left, float right, float bottom, float top, float zNear, float zFar);
	glm::mat4 viewport(float width, float height);

	void VerticesShader(unsigned int index);
	void BlinnPhongFragmentShader(std::vector<glm::vec4>& mvpv,
		std::vector<glm::vec4>& mvv,
		std::vector<glm::vec2>& pt,
		std::vector<glm::vec4>& pn);
	void Draw();
	void Draw(unsigned int index);
private:
	glm::mat4							m_modelm;
	glm::mat4							m_viewm;
	glm::mat4							m_proj;
	unsigned int						m_width;
	unsigned int						m_height;
	VertexBuffer						m_vbo;
	std::vector<OBJ::Vector3f>			m_ibo;
	std::vector<Light>					m_light;
	OBJ::Vector3f						m_camera;
	float* m_frame_buffer;
	float* m_depth_buffer;
	std::vector<float*>					m_frames_buffer;
	std::vector<float*>					m_depths_buffer;
	std::vector<Texture>				m_textures_buffer;
	std::shared_ptr<FragmentShader>		mp_fragmentshader;
	std::shared_ptr<VertexShader>		mp_vertexshader;

	bool IsInTriangle(float px, float py,
		float t0x, float t0y,
		float t1x, float t1y,
		float t2x, float t2y);
	OBJ::Vector3f BarycentricCoordinates(float xp, float yp,
		float x0, float y0,
		float x1, float y1,
		float x2, float y2);
	OBJ::Vector3f BarycentricCoordinates2(double xp, double yp,
		double x0, double y0,
		double x1, double y1,
		double x2, double y2);

	template<typename T>
	T CalBaryInter(OBJ::Vector3f& Bary, T& t0, T& t1, T& t2)
	{
		T Result;
		for (int i = 0; i < Result.size(); ++i)
			Result[i] = Bary[0] * t0[i] + Bary[1] * t1[i] + Bary[2] * t2[i];
		return Result;
	}
	template<typename T>
	T CalBaryInterGLM(OBJ::Vector3f& Bary, T& t0, T& t1, T& t2, float p0, float p1, float p2)
	{
		T Result;
		for (int i = 0; i < Result.length(); ++i)
			Result[i] = Bary[0] * t0[i] / p0 + Bary[1] * t1[i] / p1 + Bary[2] * t2[i] / p2;
		return Result;
	}
	void FindEdgePs(float* v_data,
		int v0, int v1, int v2,
		glm::vec3& blc, glm::vec3& trc);
	void FindEdgePs(std::vector<glm::vec4> mvpv,
		glm::vec3& blc,
		glm::vec3& trc);
};
}

#endif // !_RASTERIZER_H_
