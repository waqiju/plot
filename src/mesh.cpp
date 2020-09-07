#include <assert.h>
#include <glad/glad.h>
#include "mesh.h"

void Mesh::SetVertices(std::vector<float> vertices)
{
	m_Vertices = vertices;
	m_HasChanged = true;
}

void Mesh::SetIndices(std::vector<unsigned int> indices, MeshTopology topology)
{
	m_Indices = indices;
	m_Topology = topology;
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
	if (m_VAO != 0)
	{
		// TODO release previous resource
	}

	unsigned int VBO, EBO;
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// position
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), &m_Vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);
	// clean
	glBindVertexArray(0);
}

