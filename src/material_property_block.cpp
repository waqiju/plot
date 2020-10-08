#include "material_property_block.h"


const unsigned int MaterialPropertyBlock::kGLTextureNumber[10] = {
    GL_TEXTURE0,
    GL_TEXTURE1,
    GL_TEXTURE2,
    GL_TEXTURE3,
    GL_TEXTURE4,
    GL_TEXTURE5,
    GL_TEXTURE6,
    GL_TEXTURE7,
    GL_TEXTURE8,
    GL_TEXTURE9,
};


void MaterialPropertyBlock::Apply(unsigned int programID)
{
    m_TextureOrder = 0;
    for (const Property& property : m_PropertyList)
    {
        ApplyOneProperty(programID, property);
    }
}

void MaterialPropertyBlock::ApplyOneProperty(unsigned int programID, const Property& property)
{
    GLint location = glGetUniformLocation(programID, property.name.c_str());

    switch (property.kind)
    {
    case PropertyKind::Float:
        glUniform1f(location, property.floatValue);
    case PropertyKind::Color:
        glUniform4fv(location, 1, &property.colorValue.r);
    case PropertyKind::Vector3:
        glUniform3fv(location, 1, &property.vector3Value[0]);
    case PropertyKind::Matrix4x4:
        glUniformMatrix4fv(location, 1, GL_FALSE, &property.matrix4x4Value[0][0]);
    case PropertyKind::Texture:
        glActiveTexture(kGLTextureNumber[m_TextureOrder]);
        glBindTexture(GL_TEXTURE_2D, property.uintValue);
        glUniform1i(location, m_TextureOrder);
        ++m_TextureOrder;
    }
}


MaterialPropertyBlock::Property& MaterialPropertyBlock::FindOrAdd(std::string name)
{        
    auto itor = std::find_if(m_PropertyList.begin(), m_PropertyList.end(), [&](Property item) { return item.name == name; });
    if (itor != m_PropertyList.end())
    {
        return *itor;
    }
    else
    {
        m_PropertyList.push_back(Property());
        return *(m_PropertyList.end() - 1);
    }
}