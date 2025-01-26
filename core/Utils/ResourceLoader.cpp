//
// Created by Somed on 1/26/2025.
//

#include "ResourceLoader.h"

#include "ResourceManager.h"
#include "StaticDataManager.h"

void ResourceLoader::LoadProjectInfo(const char* path)
{
    std::ifstream stream;
    stream.open(path);

    if (!stream.is_open())
    {
        return;
    }

    std::string word;
    while (stream.good())
    {
        stream >> word;
        if (word == "SPRITE")
        {
            StaticDataManager::LoadSpriteInfo(stream);
        } else if (word == "ANIMATION")
        {
            StaticDataManager::LoadAnimation(stream);
        } else if (word == "STATEMACHINE")
        {
            StaticDataManager::LoadStateObject(stream);
        }
    }
    stream.close();
}

void ResourceLoader::LoadProjectInfo(const std::string& importFileName)
{
    if (ResourceManager::DataFiles.find(importFileName) != ResourceManager::DataFiles.end())
    {
        if (ResourceManager::DataFiles[importFileName].getFileType() == "import")
        {
            LoadProjectInfo(ResourceManager::DataFiles[importFileName].getFilePath().c_str());
        }
    }
}

// loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
Shader ResourceLoader::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, const std::string &name)
{
    return ResourceManager::LoadShader(vShaderFile, fShaderFile, gShaderFile, name);
}

// loads (and generates) a texture from file
Texture2D ResourceLoader::LoadTexture(const char* file, bool alpha)
{
    return ResourceManager::LoadTexture(file, alpha);
}

// loads all textures in file
void ResourceLoader::LoadTextureRecursive(const char* path, bool alpha)
{
    return ResourceManager::LoadTextureRecursive(path, alpha);
}
// loads all shaders in file
void ResourceLoader::LoadShaderRecursive(const char* path)
{
    ResourceManager::LoadShaderRecursive(path);
}
// loads all Data resources in file
void ResourceLoader::LoadDataRecursive(const char* path)
{
    ResourceManager::LoadDataRecursive(path);
}
// loads single data
FileDataWrapper ResourceLoader::LoadData(const char* path)
{
    return ResourceManager::LoadData(path);
}

//Loads default pipeline as "Default"
//static Shader loadDefaultPipeline();