#include <assert.h>
#include <glad/glad.h>
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
	material->Use();
	glBindVertexArray(mesh->VAO());
	glDrawElements(mesh->TopologyToGLenum(), mesh->IndicesCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}