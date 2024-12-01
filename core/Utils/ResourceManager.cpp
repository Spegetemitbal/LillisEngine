/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "ResourceManager.h"
#include "../EngineStuffs/Graphics/DefaultRenderPipeline.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <filesystem>

// Instantiate static variables
std::map<std::string, Texture2D>ResourceManager::Textures = std::map<std::string, Texture2D>();
std::map<std::string, Shader>ResourceManager::Shaders = std::map<std::string, Shader>();


Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

void ResourceManager::Clear()
{
    // (properly) delete all shaders	
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);
    // (properly) delete all textures
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

//Default shader is called Default
Shader ResourceManager::loadDefaultPipeline()
{
    DefaultRenderPipeline pipeline = DefaultRenderPipeline();
    const char* vShaderCode = pipeline.vertex.c_str();
    const char* fShaderCode = pipeline.fragment.c_str();
    //const char* gShaderCode = pipeline.geometry.c_str();

    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, nullptr);
    Shaders["Default"] = shader;
    return Shaders["Default"];
}

Texture2D ResourceManager::loadTextureFromFile(const char* file, bool alpha)
{
    // create texture object
    Texture2D texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    // load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    // now generate texture
    texture.Generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    return texture;
}

void ResourceManager::LoadTextureRecursive(const char* path, bool alpha) {
    if (std::filesystem::exists(path)
        && std::filesystem::is_directory(path)) {
        // Loop through each item (file or subdirectory) in
        // the directory
        for (const auto& entry :
             std::filesystem::recursive_directory_iterator(path)) {
            if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".png" || entry.path().extension() == ".jpg")
                {
                //std::cout << entry.path().stem();
                //paths.emplace_back(entry.path().filename());
                Textures[entry.path().stem().string()] = loadTextureFromFile(entry.path().string().data(), alpha);
                }
             }
        }
    else {
        // Handle the case where the directory doesn't exist
        std::cerr << "Directory not found." << std::endl;
    }
}
