//
// Created by Somed on 1/26/2025.
//

#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include "EngineStuffs/Graphics/Texture.h"
#include "EngineStuffs/Graphics/Shader.h"
#include "Utils/FileDataWrapper.h"

using namespace LILLIS;

class ResourceLoader
{
public:
    //Loads all project specific data, including settings, imports, and prefabs/animations
    static void LoadProjectInfo(const char* path);
    static void LoadProjectInfo(const std::string& importFileName);

    // loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, const std::string &name);

    // loads (and generates) a texture from file
    static Texture2D LoadTexture(const char* file, bool alpha = true);

    // loads all textures in file
    static void LoadTextureRecursive(const char* path, bool alpha = true);
    // loads all shaders in file
    static void LoadShaderRecursive(const char* path);
    // loads all Data resources in file
    static void LoadDataRecursive(const char* path);
    // loads single data
    static FileDataWrapper LoadData(const char* path);

    //Loads default pipeline as "Default"
    static Shader loadDefaultPipeline();
private:
};



#endif //RESOURCELOADER_H
