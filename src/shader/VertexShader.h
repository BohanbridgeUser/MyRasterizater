#pragma once
#ifndef _VERTEXSHADER_H_
#define	_VERTEXSHADER_H_
#include "../define.h"

namespace MyRasterizater {
class VertexShader
{
public:
	VertexShader();
	virtual ~VertexShader();

	void SetModel(Matrix4f const& rModel);
	void SetView(Matrix4f const& rView);
	void SetProj(Matrix4f const& rProj);
	void SetWidth(unsigned int ThisWidth);
	void SetHeight(unsigned int ThisHeight);

	virtual void Process(Triangle& rThisTriangle);
private:
	Matrix4f					m_modelm;
	Matrix4f					m_viewm;
	Matrix4f					m_proj;
	unsigned int				m_width;
	unsigned int				m_height;
};
}
#endif // !_VERTEXSHADER_H_
