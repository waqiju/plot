#include <assert.h>
#include "material.h"


Material::Material(Shader* shader)
{
	m_Shader = shader;
}

Material::~Material()
{
    //if (m_Shader)
    //{
    //    delete m_Shader;
    //}
}

void Material::Use()
{
	assert(m_Shader != NULL);
    m_Shader->Use();
	m_Block.Apply(m_Shader->ID);
}
