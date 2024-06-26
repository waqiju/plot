#pragma once
#include <string>
#include <vector>
#include "glad/glad.h"
#include "graphic/color.h"
#include "geometry/geometry.h"


enum class PropertyKind
{
    Float,
    Color,
    Vector3,
    Matrix4x4,
    Texture,
};


class MaterialPropertyBlock
{
    struct Property
    {
        std::string name;
        PropertyKind kind;

        // maybe union can help save memory, but it's a little
        unsigned int uintValue;
        float floatValue;
        Color colorValue;
        Vector3 vector3Value;
        Matrix4x4 matrix4x4Value;
    };
public:

    void Clear()
    {
        m_PropertyList.clear();
    }
    void SetFloat(const std::string& name, float value)
    {
        Property& p = FindOrAdd(name);
        p.name = name;
        p.kind = PropertyKind::Float;
        p.floatValue = value;
    }
    void SetColor(const std::string& name, Color value)
    {
        Property& p = FindOrAdd(name);
        p.name = name;
        p.kind = PropertyKind::Color;
        p.colorValue = value;
    }
    void SetVector3(const std::string& name, Vector3 value)
    {
        Property& p = FindOrAdd(name);
        p.name = name;
        p.kind = PropertyKind::Vector3;
        p.vector3Value = value;
    }
    void SetMatrix(const std::string& name, Matrix4x4 value)
    {
        Property& p = FindOrAdd(name);
        p.name = name;
        p.kind = PropertyKind::Matrix4x4;
        p.matrix4x4Value = value;
    }
    void SetTexture(const std::string& name, GLuint textureID)
    {
        Property& p = FindOrAdd(name);
        p.name = name;
        p.kind = PropertyKind::Texture;
        p.uintValue = textureID;
    }

    void Apply(unsigned int programID);

private:
    static const unsigned int kGLTextureNumber[10];
    std::vector<Property> m_PropertyList;
    int m_TextureOrder;

    Property& FindOrAdd(std::string name);
    void ApplyOneProperty(unsigned int programID, const Property& property);
};