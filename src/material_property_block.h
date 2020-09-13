#pragma once
#include <string>
#include <vector>
#include "glad/glad.h"
#include "geometry/geometry.h"


enum class PropertyKind
{
    Float,
    Vector3,
    Matrix4x4,
};


class MaterialPropertyBlock
{
    struct Property
    {
        std::string name;
        PropertyKind kind;
        union Value {
            float floatValue;
            Vector3 vector3Value;
            Matrix4x4 matrix4x4Value;

            Value() {}
            ~Value() {}
        } value;
    };
public:

    void Clear()
    {
        m_PropertyList.clear();
    }
    void SetFloat(const std::string& name, float value)
    {
        Property p;
        p.name = name;
        p.kind = PropertyKind::Vector3;
        p.value.floatValue = value;
        m_PropertyList.push_back(p);
    }
    void SetVector3(const std::string& name, Vector3 value)
    {
        Property p;
        p.name = name;
        p.kind = PropertyKind::Vector3;
        p.value.vector3Value = value;
        m_PropertyList.push_back(p);
    }
    void SetMatrix(const std::string& name, Matrix4x4 value)
    {
        Property p;
        p.name = name;
        p.kind = PropertyKind::Matrix4x4;
        p.value.matrix4x4Value = value;
        m_PropertyList.push_back(p);
    }

    void Apply(unsigned int programID)
    {
        for (const Property& property : m_PropertyList)
        {
            ApplyOneProperty(programID, property);
        }
    }

private:
    std::vector<Property> m_PropertyList;

    void ApplyOneProperty(unsigned int programID, const Property& property)
    {
        GLint location = glGetUniformLocation(programID, property.name.c_str());

        switch (property.kind)
        {
        case PropertyKind::Float:
            glUniform1f(location, property.value.floatValue);
        case PropertyKind::Vector3:
            glUniform3fv(location, 1, &property.value.vector3Value[0]);
        case PropertyKind::Matrix4x4:
            glUniformMatrix4fv(location, 1, GL_FALSE, &property.value.matrix4x4Value[0][0]);

        }
    }
};