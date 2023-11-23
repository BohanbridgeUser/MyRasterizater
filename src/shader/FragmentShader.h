#pragma once
#ifndef _FRAGMENTSHADER_H_
#define _FRAGMENTSHADER_H_
#include "../define.h"
#include "../Utility.h"
#include "Texture.h"

namespace MyRasterizater {
class FragmentShader
{
public:
	FragmentShader(unsigned int rId,
				   unsigned int m_width,
				   unsigned int m_height,
				   std::vector<float*>& rframe_buffer,
				   std::vector<float*>& rdepth_buffer);
	virtual ~FragmentShader()=0;
	virtual void Process(Triangle& triangle);
	void SetFrame(unsigned int id);
protected:
	void SetFrameBuffer(int y, int x, Vector3f& rColor);
	void SetDepthBuffer(int y, int x, float depth);
	float* GetDepthBuffers(unsigned int index = 0);
	float* GetFrameBuffers(unsigned int index = 0);
	float GetDepthBuffer(int y, int x);
	Texture& GetTextureBuffer(unsigned int index);
	Vector3f GetTextureValue(float u, float v);
	std::vector<Light>& GetLight();
	Vector3f& GetCamera();
private:
	unsigned int								m_frame_id;
	unsigned int								m_width;
	unsigned int								m_height;
	std::shared_ptr<std::vector<float*>>		mp_frames_buffer;
	std::shared_ptr<std::vector<float*>>		mp_depths_buffer;
	std::shared_ptr<std::vector<Texture>>		mp_textures_buffer;
	std::vector<Light>							m_light;
	Vector3f									m_camera;
};
}

#endif