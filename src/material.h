#pragma once
#include "shader.h"


class Material
{
public:
    Material(Shader* shader);
    void Use();
private:
    Shader* m_Shader = NULL;
};