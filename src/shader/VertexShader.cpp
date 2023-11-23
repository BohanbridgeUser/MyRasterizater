#include "VertexShader.h"

namespace MyRasterizater {
VertexShader::VertexShader() {}
VertexShader::~VertexShader() {}

void VertexShader::SetModel(Matrix4f const& rModel) { m_modelm = rModel; }
void VertexShader::SetView(Matrix4f const& rView) { m_modelm = rView; }
void VertexShader::SetProj(Matrix4f const& rProj) { m_modelm = rProj; }
void VertexShader::SetWidth(unsigned int ThisWidth) { m_width = ThisWidth; }
void VertexShader::SetHeight(unsigned int ThisHeight) { m_height = ThisHeight; }

void VertexShader::Process(Triangle& rThisTriangle)
{

}
}
