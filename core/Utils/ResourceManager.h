#pragma once
/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#endif

#include "glad/gl.h"
#include <filesystem>
#include "EngineStuffs/Graphics/Texture.h"
#include "EngineStuffs/Graphics/Shader.h"
#include "Utils/FileDataWrapper.h"
//#include "EngineStuffs/Audio/Sound.h"

// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no 
// public constructor is defined.

using namespace LILLIS;

enum AssetType
{
    SINGLEIMAGES = 0,
    SPRITESHEET,
    SHADERS,
    SOUNDS,
    DATA
};

class ResourceManager
{
public:
    // resource storage
    
    //FOR THE LOVE OF ALL THAT IS HOLY, MOVE THIS ELSEWHERE!!!

    static std::map<std::string, Shader>    Shaders;
    static std::map<std::string, Texture2D> SingleImages;
    static std::map<std::string, Texture2D> SpriteSheets;
    static std::map<std::string, FileDataWrapper> DataFiles;

    // loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader    LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
    // retrieves a stored sader
    static Shader    GetShader(std::string name);
    // loads (and generates) a texture from file
    static Texture2D LoadTexture(const char* file, bool alpha, bool isSpriteSheet);
    // loads all textures in file
    static void LoadTextureRecursive(const char* path, bool alpha, bool isSpriteSheet);
    // loads all shaders in file
    static void LoadShaderRecursive(const char* path);
    // loads all Data resources in file
    static void LoadDataRecursive(const char* path);
    // loads single data
    static void LoadData(const char* path);
    // retrieves a file by name
    static FileDataWrapper GetData(std::string name);
    // retrieves a stored texture
    static Texture2D GetTexture(std::string name);
    // properly de-allocates all loaded resources
    static void      Clear();
    //Loads default pipeline as "Default"
    static Shader loadDefaultPipeline();
private:
    // private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() { }
    // loads and generates a shader from file
    static Shader    loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
    // loads a single texture from file
    static Texture2D loadTextureFromFile(const char* file, bool alpha);
};