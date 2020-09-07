#include <vector>


enum class MeshTopology
{
    Triangles,
    Quads,
    Lines,
    LineStrip,
    Point,
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
private:
    void UploadMeshData();
private:
    bool m_HasChanged = false;
    unsigned int m_VAO = 0;
    MeshTopology m_Topology;

    std::vector<float> m_Vertices;
    std::vector<unsigned int> m_Indices;
};