//
// Created by Somed on 1/26/2025.
//

#ifndef STATICDATAMANAGER_H
#define STATICDATAMANAGER_H

#include <glm/vec2.hpp>

class Animation;
class StateObject;

enum SpriteType
{
    SPR_AUTO = 0,
    SPR_UNIFORM,
    SPR_MANUAL,
    SPR_CURSOR,
    SPR_WINDOW_IMAGE
};

struct TexImportData
{
    SpriteType spriteType;
    int numSprites, width, height;
    //First is height/width, then position
    std::vector<std::pair<glm::vec2, glm::vec2>> data;
};

class StaticDataManager {
public:
    static std::map<std::string, StateObject> StateObjects;
    static std::map<std::string, Animation> Animations;
    static std::map<std::string, TexImportData> SpriteInfo;
    static std::string SettingsFileName;

    static void LoadStateObject(std::ifstream& file);
    static void LoadAnimation(std::ifstream& file);
    static void LoadSpriteInfo(std::ifstream& file);
    //void LoadPrefab(std::ifstream& file);

private:

};



#endif //STATICDATAMANAGER_H
