#include "FragmentShader.h"
namespace MyRasterizater {
FragmentShader::FragmentShader(unsigned int rId,
							   unsigned int rWidth,
							   unsigned int rHeight,
							   std::vector<float*>& rframe_buffer,
							   std::vector<float*>& rdepth_buffer)
	:m_frame_id(rId),
	 m_width(rWidth),
	 m_height(rHeight),
	 mp_frames_buffer(std::make_shared<std::vector<float*>>(rframe_buffer)),
	 mp_depths_buffer(std::make_shared<std::vector<float*>>(rdepth_buffer))
{

}
FragmentShader::~FragmentShader() {}
void FragmentShader::Process(Triangle& triangle) {}
void FragmentShader::SetFrame(unsigned int id) { m_frame_id = id; }
std::vector<Light>& FragmentShader::GetLight() { return m_light; }
Vector3f& FragmentShader::GetCamera() { return m_camera; }
void FragmentShader::SetFrameBuffer(int y, int x, Vector3f& rColor)
{
	*(mp_frames_buffer->at(m_frame_id) + (m_height - 1 - y) * m_width * 3 + 3 * x) = rColor[0] * 255.f;
	*(mp_frames_buffer->at(m_frame_id) + (m_height - 1 - y) * m_width * 3 + 3 * x + 1) = rColor[0] * 255.f;
	*(mp_frames_buffer->at(m_frame_id) + (m_height - 1 - y) * m_width * 3 + 3 * x + 2) = rColor[0] * 255.f;
}
void FragmentShader::SetDepthBuffer(int y, int x, float depth)
{
	*(mp_frames_buffer->at(m_frame_id) + (m_height - 1 - y) * m_width + x) = depth;
}
float FragmentShader::GetDepthBuffer(int y, int x)
{
	return *(mp_frames_buffer->at(m_frame_id) + (m_height - 1 - y) * m_width + x);
}
Texture& FragmentShader::GetTextureBuffer(unsigned int index)
{
	return mp_textures_buffer->at(index);
}
Vector3f FragmentShader::GetTextureValue(float u, float v)
{
	return mp_textures_buffer->at(0).GetTextrue(u, v);
}
float* FragmentShader::GetDepthBuffers(unsigned int index) { return mp_depths_buffer->at(index); }
float* FragmentShader::GetFrameBuffers(unsigned int index) { return mp_frames_buffer->at(index); }
}