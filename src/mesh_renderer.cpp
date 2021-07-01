#include <assert.h>
#include <glad/glad.h>
#include "entity/ec.h"
#include "mesh_renderer.h"

MeshRenderer::MeshRenderer(Entity* owner) : Component(owner)
{
	name = "MeshRenderer";
}

void MeshRenderer::Render()
{
	assert(material != NULL);
	assert(mesh != NULL);

	mesh->CheckOrUpload();
    // 设置 MVP 矩阵
    if (camera != NULL)
    {
        Matrix4x4 mvp = camera->ViewProjectMatrix() * this->GetComponent<Transform>()->LocalToWorldMatrix();
        material->SetMatrix("MVP", mvp);
    }
    else
    {
        material->SetMatrix("MVP", Matrix4x4::identity);
    }
	material->Use();
	glBindVertexArray(mesh->VAO());
    if (mesh->EBO() != 0)
    {
        glDrawElements(mesh->TopologyToGLenum(), mesh->IndicesCount(), GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(mesh->TopologyToGLenum(), 0, mesh->VertexCount());
    }
	glBindVertexArray(0);
}
