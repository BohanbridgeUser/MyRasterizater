#pragma once
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>

#include "opencv2/opencv.hpp"

#include "OBJ_Import.h"

class Texture {
public:
	Texture(std::string const& filepath);
	~Texture();

	cv::Mat& GetTextureSource();
	OBJ::Vector3f GetTextrue(float u, float v);
private:
	std::string	 m_filepath;
	cv::Mat		 m_texture;
	unsigned int m_width;
	unsigned int m_height;
};


#endif
