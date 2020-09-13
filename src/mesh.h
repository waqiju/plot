#pragma once
#include <vector>


enum class MeshTopology
{
    Triangles,
    Quads,
    Lines,
    LineStrip,
    Points,
};


class Mesh {
public:
    void SetVertices(std::vector<float> vertices);
    void SetIndices(std::vector<unsigned int> indices, MeshTopology topology);

    void SetVertices(float vertices[], size_t length)
    {
        SetVertices(std::vector<float>(vertices, vertices + length));
    };
    void SetIndices(unsigned int indices[], size_t length, MeshTopology topology)
    {
        SetIndices(std::vector<unsigned int>(indices, indices + length), topology);
    };
    void CheckOrUpload();

    unsigned int VAO()
    {
        return m_VAO;
    }
    unsigned int EBO()
    {
        return m_EBO;
    }
    unsigned int VertexCount()
    {
        return m_Vertices.size() / 3;
    }
    GLenum TopologyToGLenum()
    {
        switch (m_Topology)
        {
        case MeshTopology::Triangles:
            return GL_TRIANGLES;
        case MeshTopology::Quads:
            return GL_QUADS;
        case MeshTopology::Lines:
            return GL_LINES;
        case MeshTopology::LineStrip:
            return GL_LINE_STRIP;
        case MeshTopology::Points:
            return GL_POINTS;
        default:
            return GL_TRIANGLES;
        }
    }
    unsigned int IndicesCount()
    {
        return m_Indices.size();
    }
private:
    void UploadMeshData();
private:
    bool m_HasChanged = false;
    unsigned int m_VAO = 0;
    unsigned int m_EBO = 0;
    MeshTopology m_Topology = MeshTopology::Triangles;

    std::vector<float> m_Vertices;
    std::vector<unsigned int> m_Indices;
};