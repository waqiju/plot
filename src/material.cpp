#include <assert.h>
#include "material.h"


Material::Material(Shader* shader)
{
	m_Shader = shader;
}

void Material::Use()
{
	assert(m_Shader != NULL);
	m_Shader->Use();
}
