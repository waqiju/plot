#pragma once
#include <GLFW/glfw3.h>
#include <string>


class AssetImporter
{
public:
    static GLuint LoadDDS(std::string texturePath);
};