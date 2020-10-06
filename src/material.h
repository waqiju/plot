#pragma once
#include <string>
#include "shader.h"
#include "geometry/geometry.h"
#include "graphic/color.h"
#include "material_property_block.h"


class Material
{
public:
    Material(Shader* shader);
    void Use();

    void SetFloat(const std::string& name, float value)
    {
        m_Block.SetFloat(name, value);
    }
    void SetColor(const std::string& name, Color value)
    {
        m_Block.SetColor(name, value);
    }
    void SetVector3(const std::string& name, Vector3 value)
    {
        m_Block.SetVector3(name, value);
    }
    void SetMatrix(const std::string& name, Matrix4x4 value)
    {
        m_Block.SetMatrix(name, value);
    }
private:
    Shader* m_Shader = NULL;
    MaterialPropertyBlock m_Block;

};