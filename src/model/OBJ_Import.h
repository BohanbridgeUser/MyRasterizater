#pragma once
#ifndef _OBJ_IMPORT_H_
#define _OBJ_IMPORT_H_

#include "../define.h"

namespace OBJ {
	typedef Eigen::Vector3f Vector3f;
	typedef Eigen::Vector2f Vector2f;
	typedef Eigen::Vector4f Vector4f;

	/**************************************************************************/
	/* Importer */
	class OBJ_Importer {
	public:
		
		enum Layout
		{
			VerticesOnly = 3,
			VerticesTextures = 6,
			VerticesTexturesNormals = 9
		};
		OBJ_Importer(std::string const& r_filepath);
		~OBJ_Importer();

		void Import();

		std::vector<Vector3f>const& GetVertices() const;
		std::vector<Vector2f>const& GetTextures() const;
		std::vector<Vector3f>const& GetNormals()  const;
		Vector3f const& GetVertex(unsigned int index) const;
		Vector2f const& GetTexture(unsigned int index)const;
		Vector3f const& GetNormal(unsigned int index) const;

		std::vector<int> const& GetFace(int index) const;
		std::vector < std::vector<int>>const& GetFaces()const;
		int GetLayout()const;
	private:
		std::ifstream							 m_file;
		std::vector<Vector3f>					 m_vert_cord;
		std::vector<Vector2f>					 m_text_cord;
		std::vector<Vector3f>					 m_norm_cord;
		std::vector<std::vector<int>>			 m_face_cord;
		Layout								     m_layout;

		Vector3f Str2Vector3f(std::string const& s, int begin);
		Vector2f Str2Vector2f(std::string const& s, int begin);
		std::vector<int> Str2Faces(std::string const& s, int begin);
	};
}


#endif
