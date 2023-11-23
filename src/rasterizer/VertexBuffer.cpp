#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
	:m_layout(BufferLayout::UNKOWN),
	 m_stride(0),
	 m_count(0),
	 m_BufferData(nullptr)
{
	m_stride = static_cast<unsigned int>(m_layout);
}
VertexBuffer::~VertexBuffer()
{
	free(m_BufferData);
}

float* VertexBuffer::GetPosition(unsigned int index)
{ 
	if (index > m_count)
	{
		std::cout << "[VertexBuffer Error] ( index out of range )" 
			<< __FILE__ << ':'
			<< __LINE__ << std::endl;
		exit(0);
	}
	return index * m_stride + m_BufferData; 
}
float* VertexBuffer::GetTexture(unsigned int index)
{
	if (m_layout == BufferLayout::V) {
		std::cout << "[VertexBuffer Error] ( Failed get Texture coordinates ) "
			<< __FILE__ << ':'
			<< __LINE__ << std::endl;
		return nullptr;
	}
	return index * m_stride + 4 + m_BufferData;
}
float* VertexBuffer::GetNormal(unsigned int index)
{
	if (m_layout != BufferLayout::VTN) {
		std::cout << "[VertexBuffer Error] ( Failed get Normal coordinates ) "
			<< __FILE__ << ':'
			<< __LINE__ << std::endl;
		return nullptr;
	}
	return index * m_stride + 6 + m_BufferData;
}
float* VertexBuffer::GetBufferData() { return m_BufferData; }
unsigned int VertexBuffer::GetStride() const { return m_stride; }
unsigned int VertexBuffer::GetCount() const { return m_count; }
void VertexBuffer::SetBufferData(float* p_ThisBuffer) { m_BufferData = p_ThisBuffer; }
void VertexBuffer::ImportData(OBJ::OBJ_Importer& model)
{
	int layout = model.GetLayout();
	auto& vertices = model.GetVertices();
	auto& textures = model.GetTextures();
	auto& normals = model.GetNormals();
	auto& faces = model.GetFaces();
	unsigned int cntoffloat = 0;
	m_count = faces.size() * 3;
	switch (layout)
	{
		case 3:
			m_layout = BufferLayout::V;
			m_stride = 4;
			cntoffloat = faces.size() * 4 * 3;
			break;
		case 6:
			m_layout = BufferLayout::VT;
			m_stride = 6;
			cntoffloat = faces.size() * 6 * 3;
			break;
		case 9:
			m_layout = BufferLayout::VTN;
			m_stride = 9;
			cntoffloat = faces.size() * 9 * 3;
			break;
	}
	m_BufferData = (float*)malloc(cntoffloat * sizeof(float));
	if (!m_BufferData) {
		std::cout << "[Vertex Buffer] (Buffer Data storage allocate failed! )\n";
	}
	memset(m_BufferData, 0.00f, cntoffloat * sizeof(float));
	int cnt = 0;
	for (int i = 0; i < faces.size(); ++i)
	{
		auto& face = faces[i];
		int j = 0;
		for(;j<face.size() && vertices.size() != 0;) {
			m_BufferData[cnt++] = vertices[face[j]][0];
			m_BufferData[cnt++] = vertices[face[j]][1];
			m_BufferData[cnt++] = vertices[face[j]][2];
			m_BufferData[cnt++] = 1.00f;
			j++;
			if (textures.size() != 0) {
				m_BufferData[cnt++] = textures[face[j]][0];
				m_BufferData[cnt++] = textures[face[j]][1];
				j++;
			}
			if (normals.size() != 0) {
				m_BufferData[cnt++] = normals[face[j]][0];
				m_BufferData[cnt++] = normals[face[j]][1];
				m_BufferData[cnt++] = normals[face[j]][2];
				j++;
			}
		}	
	}
	/*std::ofstream file("Vertex Buffer input");
	for (int i = 0; i < m_count; ++i)
	{
		file << (float)m_BufferData[m_stride * i] << ' '
			<< (float)m_BufferData[m_stride * i + 1] << ' '
			<< (float)m_BufferData[m_stride * i + 2] << ' '
			<< (float)m_BufferData[m_stride * i + 3] << ' '
			<< (float)m_BufferData[m_stride * i + 4] << ' '
			<< (float)m_BufferData[m_stride * i + 5] << ' '
			<< (float)m_BufferData[m_stride * i + 6] << ' '
			<< (float)m_BufferData[m_stride * i + 7] << ' '
			<< (float)m_BufferData[m_stride * i + 8] << std::endl;
	}
	file.close();*/
}

