#pragma once
#ifndef _VERTEXBUFFER_H_
#define _VERTEXBUFFER_H_

#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>

#include "OBJ_Import.h"

/**
* @brief VerticesOnly:
						float[0]~flaot[3] position;
*		 VertivesAndTextures:
						float[0]~flaot[3] position;
						float[4]~float[5] texture_coordinates;
			All
						float[0]~flaot[3] position;
						float[4]~float[5] texture_coordinates;
						float[6]~float[8] normal;
*/
enum class BufferLayout
{
	UNKOWN = -1,
	V = 4,
	VT = 6,
	VTN = 9
};

class VertexBuffer {
	
public:
	VertexBuffer();
	~VertexBuffer();

	
	float* GetPosition(unsigned int index);
	float* GetTexture(unsigned int index);
	float* GetNormal(unsigned int index);
	float* GetBufferData();
	unsigned int GetCount() const;
	unsigned int GetStride() const;

	void SetBufferData(float* p_ThisBuffer);
	void ImportData(OBJ::OBJ_Importer& model);
	
private:
	BufferLayout							  m_layout;
	unsigned int							  m_count;
	unsigned int							  m_stride;
	float*									  m_BufferData;
};

#endif
