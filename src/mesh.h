#pragma once
#include <vector>
#include "gl_headers.h"
#include "geometry/geometry.h"
#include "graphic/graphic.h"


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
    Mesh();
    ~Mesh();

    void SetVertices(std::vector<float> vertices);
    void SetVertices(std::vector<Vector3> vertices);
    void SetVertices(float vertices[], size_t length)
    {
        SetVertices(std::vector<float>(vertices, vertices + length));
    }

    void SetIndices(std::vector<unsigned int> indices, MeshTopology topology);
    void SetIndices(unsigned int indices[], size_t length, MeshTopology topology)
    {
        SetIndices(std::vector<unsigned int>(indices, indices + length), topology);
    }
    void SetColors(const std::vector<Color>& colors);
    void SetTopology(MeshTopology topology) { m_Topology = topology; }
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
    unsigned int m_VboPosition = 0;
    unsigned int m_VboColor = 0;
    unsigned int m_EBO = 0;
    MeshTopology m_Topology = MeshTopology::Triangles;

    std::vector<float> m_Vertices;
    std::vector<Color> m_Colors;
    std::vector<unsigned int> m_Indices;
};