#pragma once
#ifndef _TRANSFORMMATRIX_H_
#define _TRANSFORMMATRIX_H_

#include <iostream>
#include <cmath>

#include "glm/glm.hpp"

class TransformMatrix {
public:
	static glm::mat4 Scale(glm::mat4 initmat, glm::vec3 scaler3)
	{
		glm::vec4 scaler4 = glm::vec4(scaler3, 1.0f);
		for (int i = 0; i < 4; ++i)
			initmat[i][i] *= scaler4[i];
		return initmat;
	}
	static glm::mat4 Translate(glm::mat4 initmat, glm::vec3 translater)
	{
		initmat[3] = initmat[0]*translater[0]
					 + initmat[1]*translater[1]
					 + initmat[2]*translater[2]
					 + initmat[3];
		return initmat;
	}
	static glm::mat4 Rotation(glm::mat4 initmat, float roangle, glm::vec3 k)
	{
		glm::mat3 kkt;
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; j++)
				kkt[j][i] = k[i] * k[j];
		
		glm::mat3 nt(1.0f);
		nt[0][0] =  0.0f; nt[1][0] = -k[2]; nt[2][0] =  k[1];
		nt[0][1] =  k[2]; nt[1][1] =  0.0f; nt[2][1] = -k[0];
		nt[0][2] = -k[1]; nt[1][2] =  k[0]; nt[2][2] =  0.0f;
		glm::mat3 Mr = std::cos(roangle) * glm::mat3(1.0f)
					   + (1 - std::cos(roangle)) * kkt
					   + std::sin(roangle) * nt;
		glm::mat4 ret(Mr);
		ret[3][3] = 1.0f;
		ret *= initmat;
		return ret;
	}
	static glm::mat4 Perspective(float fov, float aspect, float zNear, float zFar)
	{
		glm::mat4 persproj(0.0f);
		const float tanHalffov = std::tan(fov / 2.00f);
		persproj[0][0] = 1.00f / (aspect * tanHalffov);
		persproj[1][1] = 1.00f / (tanHalffov);
		persproj[2][2] = -(zFar + zNear) / (zFar - zNear);
		persproj[3][2] = -2 * (zFar * zNear) / (zFar - zNear);
		persproj[2][3] = -1;
		return persproj;
	}
	static glm::mat4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		glm::mat4 ortho(1.0f);
		ortho[0][0] = 2.00 / (right - left);
		ortho[1][1] = 2.00 / (right - left);
		ortho[3][0] = -(right + left) / (right - left);
		ortho[3][1] = -(bottom + top) / (right - left);
		ortho[2][2] = -2.00 / (zFar - zNear);
		ortho[3][2] = -(zFar + zNear) / (zFar - zNear);
		return ortho;
	}
private:

};



#endif

