#include "OBJ_Import.h"

namespace OBJ {
	/**************************************************************************/
	/* OBJ_Importer */
	OBJ_Importer::OBJ_Importer(std::string const& r_filepath)
	{
		m_file.open(r_filepath);
		if (!m_file.is_open())
		{
			std::cout << "[OBJ_Importer Error] (" << r_filepath << ") open failed! \n";
			__debugbreak();
		}
	}
	OBJ_Importer::~OBJ_Importer()
	{
		m_file.close();
	}

	void OBJ_Importer::Import()
	{
		std::string line;
		while (getline(m_file, line))
		{
			if (line[0] == 'v' && line[1] == ' ')
				m_vert_cord.push_back(Str2Vector3f(line, 2));
			if (line[0] == 'v' && line[1] == 't')
				m_text_cord.push_back(Str2Vector2f(line, 3));
			if (line[0] == 'v' && line[1] == 'n')
				m_norm_cord.push_back(Str2Vector3f(line, 3));
			if (line[0] == 'f' && line[1] == ' ')
				m_face_cord.push_back(Str2Faces(line, 2));	
		}
		if (m_face_cord.size() == 0)
			std::cout << "[OBJ_Import Error] ( 0 Faces read )\n";
		m_layout = static_cast<Layout>(m_face_cord[0].size());
	}

	Vector3f OBJ_Importer::Str2Vector3f(std::string const& s, int begin)
	{
		int end = begin;
		float loc[3] = { 0.0f };
		for (int i = 0; i < 3 && end < s.size(); ++i)
		{
			while (end < s.size() && s[end] != ' ') end++;
			std::string temp(s.begin() + begin, s.begin() + end);
			loc[i] = stof(temp);
			begin = end + 1;
			end = begin;
		}
		return Vector3f(loc[0], loc[1], loc[2]);
	}
	Vector2f OBJ_Importer::Str2Vector2f(std::string const& s, int begin)
	{
		int end = begin;
		float loc[2] = { 0.0f };
		for (int i = 0; i < 2 && end < s.size(); ++i)
		{
			while (end < s.size() && s[end] != ' ') end++;
			std::string temp(s.begin() + begin, s.begin() + end);
			loc[i] = stof(temp);
			begin = end + 1;
			end = begin;
		}
		return Vector2f(loc[0], loc[1]);
	}
	std::vector<int> OBJ_Importer::Str2Faces(std::string const& s, int begin)
	{
		int end = begin;
		int len = s.length();
		std::vector<int> face;
		while(end < len)
		{
			while (end < len && s[end] != ' ')
			{
				unsigned int index = 0;
				while (end < s.size() && s[end] != '/' && s[end] != ' ')
				{
					index = index * 10 + (s[end] - '0');
					end++;
				}
				face.push_back(index - 1);
				if (s[end] == '/')end++;
			}
			begin = end + 1;
			end = begin;
		}
		return face;
	}

	std::vector<Vector3f> const& OBJ_Importer::GetVertices()const { return m_vert_cord; }
	std::vector<Vector2f> const& OBJ_Importer::GetTextures()const { return m_text_cord; }
	std::vector<Vector3f> const& OBJ_Importer::GetNormals()const { return m_norm_cord; }
	Vector3f const& OBJ_Importer::GetVertex(unsigned int index) const { return m_vert_cord[index]; }
	Vector2f const& OBJ_Importer::GetTexture(unsigned int index)const { return m_text_cord[index]; }
	Vector3f const& OBJ_Importer::GetNormal(unsigned int index) const { return m_norm_cord[index]; }
	std::vector<int>const& OBJ_Importer::GetFace(int index)const { return m_face_cord[index]; }
	std::vector < std::vector<int>>const& OBJ_Importer::GetFaces() const { return m_face_cord; }
	int OBJ_Importer::GetLayout()const { return static_cast<int>(m_layout); }
}