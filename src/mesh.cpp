#include <assert.h>
#include <glad/glad.h>
#include "mesh.h"

Mesh::Mesh()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VboPosition);
}

Mesh::~Mesh()
{
	if (m_VAO)
	{
		glDeleteVertexArrays(1, &m_VAO);
	}
	if (m_VboPosition)
	{
		glDeleteBuffers(1, &m_VboPosition);
	}
	if (m_VboColor)
	{
		glDeleteBuffers(1, &m_VboColor);
	}
	if (m_VboUV)
	{
		glDeleteBuffers(1, &m_VboUV);
	}
	if (m_EBO)
	{
		glDeleteBuffers(1, &m_EBO);
	}
}

void Mesh::SetVertices(std::vector<float> vertices)
{
	m_Vertices = vertices;
	m_HasChanged = true;
}

void Mesh::SetVertices(std::vector<Vector3> vertices)
{
	m_Vertices.clear();
	for (const Vector3& v : vertices)
	{
		m_Vertices.push_back(v.x);
		m_Vertices.push_back(v.y);
		m_Vertices.push_back(v.z);
	}

	m_HasChanged = true;
}

void Mesh::SetIndices(std::vector<unsigned int> indices, MeshTopology topology)
{
	m_Indices = indices;
	m_Topology = topology;
	m_HasChanged = true;
}

void Mesh::SetColors(const std::vector<Color>& colors)
{
	m_Colors = colors;
	m_HasChanged = true;
}

void Mesh::SetUVs(const std::vector<Vector2>& uvs)
{
	m_UVs = uvs;
	m_HasChanged = true;
}

void Mesh::CheckOrUpload()
{
	if (m_HasChanged)
	{
		UploadMeshData();
		m_HasChanged = false;
	}
	assert(m_VAO != 0);
}

void Mesh::UploadMeshData()
{
	// 非法数据保护
	if (m_Vertices.size() == 0)
		return;

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VboPosition);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), &m_Vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// colors
	if (m_Colors.size() > 0)
	{
		if (m_VboColor == 0)
		{
			glGenBuffers(1, &m_VboColor);
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_VboColor);
		glBufferData(GL_ARRAY_BUFFER, m_Colors.size() * sizeof(Color), &m_Colors[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Color), (void*)0);
	}
	// uvs
	if (m_UVs.size() > 0)
	{
		if (m_VboUV == 0)
		{
			glGenBuffers(1, &m_VboUV);
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_VboUV);
		glBufferData(GL_ARRAY_BUFFER, m_UVs.size() * sizeof(Vector2), &m_UVs[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), (void*)0);
	}
	// indices
	if (m_Indices.size() > 0)
	{
		if (m_EBO == 0)
		{
			glGenBuffers(1, &m_EBO);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);
	}
	// clean
	glBindVertexArray(0);
}

