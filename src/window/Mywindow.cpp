#include "Mywindow.h"

Mywindow::Mywindow(std::string const& name)
	:m_name(name),
	 m_image(cv::Mat()){}
Mywindow::~Mywindow(){}

void Mywindow::SetImage(cv::Mat rimage)
{ 
	m_image = rimage.clone(); 
	m_image.convertTo(m_image, CV_8UC3, 1.0f);
	cv::cvtColor(m_image, m_image, cv::COLOR_RGB2BGR);
}
void Mywindow::SetName(std::string const& name) { m_name = name; }

int Mywindow::Key() { return cv::waitKey(10); }
void Mywindow::display() const 
{
	cv::namedWindow(m_name, cv::WINDOW_FREERATIO);
	cv::imshow(m_name, m_image); 
}

void Mywindow::Begin()
{
	int key = 0;
	while (key != 27)
	{
		display();
		key = Key();
	}
}