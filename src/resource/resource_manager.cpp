/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include "asset_importer.h"

// Instantiate static variables
std::map<std::string, Texture2D*>    ResourceManager::Textures;
std::map<std::string, Shader*>       ResourceManager::Shaders;


Shader* ResourceManager::LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name)
{
    if (Shaders.find(name) == Shaders.end())
    {
        Shaders[name] = new Shader(vShaderFile, fShaderFile);
    }
    return Shaders[name];
}

Shader* ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture2D* ResourceManager::LoadTexture(const char *file, bool alpha, std::string name)
{
    if (Textures.find(name) == Textures.end())
    {
        Textures[name] = AssetImporter::LoadTexture2D(file, alpha);
    }
    return Textures[name];
}

Texture2D* ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

void ResourceManager::Clear()
{
    // (properly) delete all shaders	
    for (auto iter : Shaders)
        glDeleteProgram(iter.second->ID);
    // (properly) delete all textures
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second->ID);
}
