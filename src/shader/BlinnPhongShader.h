#pragma once
#ifndef _BLINNPHONGSHADER_H_
#define _BLINNPHONGSHADER_H_
#include "FragmentShader.h"
namespace MyRasterizater {
class BlinnPhongShader : public FragmentShader
{
public:
	BlinnPhongShader(unsigned int rId,
					 unsigned int m_width,
					 unsigned int m_height,
					 std::vector<float*>& rframe_buffer,
					 std::vector<float*>& rdepth_buffer);
	virtual ~BlinnPhongShader();
	virtual void Process(Triangle& triangle) override;
	void SetLightAmbient(Vector3f& ambient);
	void SetKa(Vector3f& rka);
	void SetKs(Vector3f& rks);
private:
	Vector3f lightambient = { 10.0f, 10.0f, 10.0f };
	Vector3f ka = { 0.005f, 0.005f, 0.005f };
	Vector3f ks = { 0.7937f, 0.7937f, 0.7937f };
};
}
#endif
