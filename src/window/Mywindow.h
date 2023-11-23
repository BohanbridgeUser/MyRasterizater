#pragma once
#ifndef _MYWINDOW_H_
#define _MYWINDOW_H_

#include <string>

#include "opencv2/opencv.hpp"

class Mywindow{
public:
	Mywindow(std::string const& name);
	~Mywindow();

	void display() const;
	void SetImage(cv::Mat rimage);
	void SetName(std::string const& name);
	
	void Begin();
private:
	std::string m_name;
	cv::Mat		m_image;
	int Key();
};

#endif
