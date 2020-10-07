#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include "texture.h"


class AssetImporter
{
public:
    static GLuint LoadDDS(std::string texturePath);
    static Texture2D* LoadTexture2D(const std::string& texturePath, bool alpha);
};