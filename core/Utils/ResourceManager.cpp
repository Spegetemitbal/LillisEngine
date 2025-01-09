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
#include "../EngineStuffs/StateObject.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <filesystem>
#include <utility>

// Instantiate static variables
std::map<std::string, Texture2D>ResourceManager::SpriteTexs = std::map<std::string, Texture2D>();
std::map<std::string, Shader>ResourceManager::Shaders = std::map<std::string, Shader>();
std::map<std::string, FileDataWrapper>ResourceManager::DataFiles = std::map<std::string, FileDataWrapper>();
std::map<std::string, TexImportData> ResourceManager::SpriteInfo = std::map<std::string, TexImportData>();
std::map<std::string, Animation> ResourceManager::Animations = std::map<std::string, Animation>();
std::map<std::string, StateObject> ResourceManager::StateObjects = std::map<std::string, StateObject>();
std::string ResourceManager::SettingsFileName;


Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader ResourceManager::GetShader(const std::string& name)
{
    if (Shaders.find(name) != Shaders.end())
    {
        return Shaders[name];
    }
    return {};
}

Texture2D ResourceManager::LoadTexture(const char* file, bool alpha)
{
    if (SpriteInfo.empty())
    {
        std::cout << "Import data required for reading";
        return {};
    }
    std::filesystem::path path(file);
    SpriteTexs[path.stem().string()] = loadTextureFromFile(path.stem().string().data(),file, alpha);
    return SpriteTexs[path.stem().string()];
}

Texture2D ResourceManager::GetTexture(const std::string& name)
{
    if (SpriteTexs.find(name) != SpriteTexs.end())
    {
        return SpriteTexs[name];
    } else
    {
        std::cout << name << " not found" << std::endl;
        return {};
    }
}

void ResourceManager::Clear()
{
    // (properly) delete all shaders	
    for (const auto& iter : Shaders)
        glDeleteProgram(iter.second.ID);
    // (properly) delete all textures
    for (const auto& iter : SpriteTexs)
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

Texture2D ResourceManager::loadTextureFromFile(const char* name,const char* file, bool alpha)
{
    if (!SpriteInfo.contains(name))
    {
        std::cout << "No import data found, unable to import";
        return {};
    }

    TexImportData importData = SpriteInfo[name];

    // create texture object
    Texture2D texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    // load image
    int width, height, nrChannels;
    //stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    // now generate texture
    texture.Generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);

    int numRead = 0;
    switch (importData.spriteType)
    {
        case SPR_AUTO:
            texture.spriteLocations.emplace_back(0.0f,1.0f,0.0f,1.0f);
            texture.spriteSizes.emplace_back(width, height);
            break;
        case SPR_UNIFORM:
            glm::vec2 spriteGrid = {1.0f / (float)importData.width, 1.0f / (float)importData.height};
            for (int i = 0; i < importData.height; i++)
            {
                for (int j = 0; j < importData.width; j++)
                {
                    if (numRead >= importData.numSprites)
                    {
                        break;
                    }
                    float x = spriteGrid.x * (float)j;
                    float y = spriteGrid.y * (float)i;
                    texture.spriteLocations.emplace_back(x, x + spriteGrid.x, y, y + spriteGrid.y);
                    texture.spriteSizes.emplace_back(spriteGrid.x * (float)width, spriteGrid.y * (float)height);
                    numRead++;
                }
                if (numRead == importData.numSprites)
                {
                    break;
                }
            }
            break;
        case SPR_MANUAL:
            break;
    }

    return texture;
}

void ResourceManager::LoadTextureRecursive(const char* path, bool alpha) {
    if (SpriteInfo.empty())
    {
        std::cout << "Import data required for reading";
        return;
    }

    if (std::filesystem::exists(path)
        && std::filesystem::is_directory(path)) {
        // Loop through each item (file or subdirectory) in
        // the directory
        for (const auto& entry :
             std::filesystem::recursive_directory_iterator(path)) {
            if (std::filesystem::is_regular_file(entry) && entry.path().extension().string() == ".png" || entry.path().extension().string() == ".jpg")
                {
                    SpriteTexs[entry.path().stem().string()] = loadTextureFromFile(entry.path().stem().string().data(),entry.path().string().data(), alpha);
                }
             }
        }
    else {
        // Handle the case where the directory doesn't exist
        std::cerr << "Directory not found." << std::endl;
    }
}

void ResourceManager::LoadShaderRecursive(const char* path)
{
    std::vector<std::filesystem::path> shaders = std::vector<std::filesystem::path>();
    std::vector<std::string> shaderNames = std::vector<std::string>();
    std::multimap<std::string, std::filesystem::path> shaderChunks = std::multimap<std::string, std::filesystem::path>();
    if (std::filesystem::exists(path)
    && std::filesystem::is_directory(path))
    {
        // Loop through each item (file or subdirectory) in
        // the directory
        for (const auto& entry :
             std::filesystem::recursive_directory_iterator(path))
        {
            if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".frag"
                || entry.path().extension() == ".vert" || entry.path().extension() == ".geom")
            {
                shaders.push_back(entry.path());
            }
        }
    }
    else {
        // Handle the case where the directory doesn't exist
        std::cerr << "Directory not found." << std::endl;
    }

    //First sort by names
    for (int i = 0; i < shaders.size(); i++)
    {
        if (std::find(shaderNames.begin(), shaderNames.end(), shaders[i].stem().string()) == shaderNames.end())
        {
            shaderNames.push_back(shaders[i].stem().string());
        }
        shaderChunks.insert(std::pair<std::string, std::filesystem::path>(shaders[i].stem().string(), shaders[i]));
    }

    //Then determine if there are sufficient shaders and input!
    for (int i = 0; i < shaderNames.size(); i++)
    {
        std::string name = shaderNames[i];

        std::filesystem::path fragment, vertex, geometry;
        auto range = shaderChunks.equal_range(name);
        for (auto j = range.first; j != range.second; ++j)
        {
            if (j->second.extension().string() == ".frag")
            {
                fragment = j->second;
            } else if (j->second.extension().string() == ".vert")
            {
                vertex = j->second;
            } else
            {
                geometry = j->second;
            }
        }

        if (!fragment.empty() && !vertex.empty())
        {
            if (!geometry.empty())
            {
                loadShaderFromFile(vertex.string().c_str(), fragment.string().c_str(), geometry.string().c_str());
            } else
            {
                loadShaderFromFile(vertex.string().c_str(), fragment.string().c_str(), nullptr);
            }
        } else
        {
            std::cout << "Shader not found for pipeline: " << name << std::endl;
        }

    }


}

FileDataWrapper ResourceManager::GetData(const std::string& name)
{
    if (DataFiles.find(name) != DataFiles.end())
    {
        return DataFiles[name];
    }
    return {};
}

//Currently will load anything you want, use sparingly.
FileDataWrapper ResourceManager::LoadData(const char *path)
{
    std::filesystem::path resourcePath(path);
    try
    {
        if (DataFiles.contains(resourcePath.stem().string()))
        {
            return DataFiles[resourcePath.stem().string()];
        }
        DataFiles[resourcePath.stem().string()] = FileDataWrapper(path);
        return DataFiles[resourcePath.stem().string()];
    } catch (...)
    {
        std::cerr << "Invalid File" << std::endl;
        return {};
    }
}


//Will technically load... anything. Please keep your data files separate from other assets.
void ResourceManager::LoadDataRecursive(const char *path)
{
    if (std::filesystem::exists(path)
        && std::filesystem::is_directory(path)) {
        // Loop through each item (file or subdirectory) in
        // the directory
        for (const auto& entry :
             std::filesystem::recursive_directory_iterator(path))
            {
                if (std::filesystem::is_regular_file(entry))
                {
                    DataFiles[entry.path().stem().string()] = FileDataWrapper(entry.path().string().c_str());
                }
            }
        }
    else {
        // Handle the case where the directory doesn't exist
        std::cerr << "Directory not found." << std::endl;
    }
}

void ResourceManager::LoadProjectInfo(const char* path)
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
            std::string name, importType;
            stream >> name;
            stream >> importType;
            SpriteInfo.emplace(name, TexImportData());

            if (importType == "Auto")
            {
                SpriteInfo[name].spriteType = SPR_AUTO;
                SpriteInfo[name].numSprites = 1;
            } else if (importType == "Uniform")
            {
                int numSprites, width, height;
                stream >> numSprites;
                stream >> width;
                stream >> height;
                SpriteInfo[name].spriteType = SPR_UNIFORM;
                SpriteInfo[name].numSprites = numSprites;
                SpriteInfo[name].width = width;
                SpriteInfo[name].height = height;
            } else if (importType == "Manual")
            {
                std::cout << "Manual size spritesheet not implemented yet, ending load";
                break;
            }
        } else if (word == "ANIMATION")
        {
            std::string name, repType;
            RepeatType repeatType = REPEAT_STOP;
            int numKeyFrames;
            stream >> name;
            stream >> numKeyFrames;
            stream >> repType;
            if (repType == "LOOP")
            {
                repeatType = REPEAT_LOOP;
            } else if (repType == "CLAMP")
            {
                repeatType = REPEAT_CLAMP;
            }
            Animations.emplace(name, Animation(repeatType));

            for (int i = 0; i < numKeyFrames; i++)
            {
                std::string kFrameCheck;
                stream >> kFrameCheck;
                if (kFrameCheck == "KeyFrame")
                {
                    float time;
                    int numParams;
                    stream >> time;
                    stream >> numParams;
                    Animations[name].insertKeyFrame({time});
                    for (int j = 0; j < numParams; j++)
                    {
                        std::string paramType;
                        stream >> paramType;
                        if (paramType == "Sprite")
                        {
                            std::string spriteImage;
                            int frame;
                            float offX, offY, szX, szY;
                            KeyFrame& keyFrame = Animations[name].getKeyFrame(i);
                            stream >> spriteImage;
                            stream >> frame;
                            stream >> offX;
                            stream >> offY;
                            stream >> szX;
                            stream >> szY;
                            keyFrame.hasSpriteData = true;
                            keyFrame.fsd.sprImage = spriteImage;
                            keyFrame.fsd.sprFrame = frame;
                            keyFrame.fsd.sprOffset = {offX, offY};
                            keyFrame.fsd.sprSize = {szX, szY};
                        } else if (paramType == "Collider")
                        {
                            std::cout << "Collider param not Implemented yet";
                        } else if (paramType == "Transform")
                        {
                            std::cout << "Transform param not Implemented yet";
                        }
                    }
                }
            }
        } else if (word == "STATEMACHINE")
        {
            std::string name;
            int numStates;
            stream >> name;
            stream >> numStates;
            StateObjects.emplace(name, StateObject(name));
            for (int i = 0; i < numStates; i++)
            {
                LilState mState;
                std::string item, stateName, animName;
                double maxTime;
                int numToStates;
                stream >> item;
                stream >> stateName;
                stream >> animName;
                stream >> maxTime;
                stream >> numToStates;
                if (item == "State")
                {
                    if (i == 0)
                    {
                        StateObjects[name].SetDefaultState(stateName);
                    }
                    mState.name = stateName;
                    mState.anim = &Animations[animName];
                    mState.maxStateTime = maxTime;
                    for (int j = 0; j < numToStates; j++)
                    {
                        std::string toStateName, frItem;
                        int cond;
                        stream >> frItem;
                        stream >> toStateName;
                        stream >> cond;
                        if (frItem == "ToState")
                        {
                            mState.toStates.emplace(toStateName, (LilStateChangeConditions)cond);
                            StateObjects[name].AddNewState(mState);
                        }
                    }

                }
            }
        }
    }
    stream.close();
}

void ResourceManager::LoadProjectInfo(const std::string& importFileName)
{
    if (DataFiles.find(importFileName) != DataFiles.end())
    {
        if (DataFiles[importFileName].getFileType() == "import")
        {
            LoadProjectInfo(DataFiles[importFileName].getFilePath().c_str());
        }
    }
}

