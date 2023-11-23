#pragma once
#ifndef _DEFINE_H_

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <cmath>
#include <fstream>
#include <Eigen/Eigen>

namespace MyRasterizater {
	typedef Eigen::Matrix4f Matrix4f;
	typedef Eigen::Matrix3f Matrix3f;
	typedef Eigen::Vector3f Vector3f;
	typedef Eigen::Vector4f Vector4f;
	typedef Eigen::Vector2f Vector2f;

	struct Triangle
	{
		std::vector<Vector4f>	m_f_posi;
		std::vector<Vector4f>   m_b_posi;
		std::vector<Vector3f>	m_text;
		std::vector<Vector3f>	m_norm;
	};
	struct Light
	{
		Vector3f position;
		Vector3f intensity;
	};
}

#endif // !_DEFINE_H_
