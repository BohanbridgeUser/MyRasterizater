#include "Texture.h"

Texture::Texture(std::string const& filepath)
	:m_filepath(filepath)
{
	m_texture = cv::imread(filepath);
	if (m_texture.empty()) {
		std::cout << "[ Texture Error ] ( Texture read failed )"
			<< __FILE__ << ":" << __LINE__ << std::endl;
		exit(0);
	}
	else
	{
		std::cout << "[ Texture ] ( Texture read successed ) "
			<< "Mat type : " << m_texture.type() << std::endl
			<< m_texture.cols << ' ' << m_texture.rows << std::endl;
		std::cout << "CV_8UC1 " << CV_8UC1 << std::endl
			<< "CV_8UC2 " << CV_8UC2 << std::endl
			<< "CV_8UC3 " << CV_8UC3 << std::endl
			<< "CV_8UC4 " << CV_8UC4 << std::endl
			<< "CV_8SC1 " << CV_8SC1 << std::endl
			<< "CV_8SC2 " << CV_8SC2 << std::endl
			<< "CV_8SC3 " << CV_8SC3 << std::endl
			<< "CV_8SC4 " << CV_8SC4 << std::endl
			<< "CV_16UC1 " << CV_16UC1 << std::endl
			<< "CV_16UC2 " << CV_16UC2 << std::endl
			<< "CV_16UC3 " << CV_16UC3 << std::endl
			<< "CV_16UC4 " << CV_16UC4 << std::endl

			<< "CV_16SC1 " << CV_16SC1 << std::endl
			<< "CV_16SC2 " << CV_16SC2 << std::endl
			<< "CV_16SC3 " << CV_16SC3 << std::endl
			<< "CV_16SC4 " << CV_16SC4 << std::endl

			<< "CV_32FC1 " << CV_32FC1 << std::endl
			<< "CV_32FC2 " << CV_32FC2 << std::endl
			<< "CV_32FC3 " << CV_32FC3 << std::endl
			<< "CV_32FC4 " << CV_32FC4 << std::endl

			<< "CV_64FC1 " << CV_64FC1 << std::endl
			<< "CV_64FC2 " << CV_64FC2 << std::endl
			<< "CV_64FC3 " << CV_64FC3 << std::endl
			<< "CV_64FC4 " << CV_64FC4 << std::endl

			<< "CV_16FC1 " << CV_16FC1 << std::endl
			<< "CV_16FC2 " << CV_16FC2 << std::endl
			<< "CV_16FC3 " << CV_16FC3 << std::endl
			<< "CV_16FC4 " << CV_16FC4 << std::endl;

		/*if (m_texture.type() == CV_8UC3) {
			int key = 0;
			while (key != 27)
			{
				cv::namedWindow("m_name", cv::WINDOW_FREERATIO);
				cv::imshow("m_name", m_texture);
				cv::waitKey(10);
			}
		}*/
	}
	m_width = m_texture.cols;
	m_height = m_texture.rows;
	cv::cvtColor(m_texture, m_texture, cv::COLOR_RGB2BGR);
}
Texture::~Texture()
{
}
/* the input u,v in range[0,1] */
OBJ::Vector3f Texture::GetTextrue(float u, float v)
{
	if (u < 0) u = 0;
	if (u > 1) u = 1;
	if (v > 1) v = 1;
	if (v < 0) v = 0;

	float u2o = (1 - v) * m_width, v2o = u * m_height;
	using std::floor;
	using std::ceil;
	u2o = floor(u2o);
	v2o = floor(v2o);
	unsigned char* p = m_texture.ptr<unsigned char>(u2o);
	int lv0 = 3 * v2o, lv1 = 3 * v2o + 1, lv2 = 3 * v2o + 2;
	return OBJ::Vector3f(p[lv0],p[lv1],p[lv2]);
}
cv::Mat& Texture::GetTextureSource()
{
	return m_texture;
}