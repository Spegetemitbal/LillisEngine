//
// Created by Somed on 1/26/2025.
//

#include "StaticDataManager.h"
#include "../EngineStuffs/StateObject.h"

std::map<std::string, TexImportData> StaticDataManager::SpriteInfo = std::map<std::string, TexImportData>();
std::map<std::string, Animation> StaticDataManager::Animations = std::map<std::string, Animation>();
std::map<std::string, StateObject> StaticDataManager::StateObjects = std::map<std::string, StateObject>();
std::string StaticDataManager::SettingsFileName;

void StaticDataManager::LoadStateObject(std::ifstream& file)
{
    std::string name;
    int numStates;
    file >> name;
    file >> numStates;
    StateObjects.emplace(name, StateObject(name));
    for (int i = 0; i < numStates; i++)
    {
        LilState mState;
        std::string item, stateName, animName;
        double maxTime;
        int numToStates;
        file >> item;
        file >> stateName;
        file >> animName;
        file >> maxTime;
        file >> numToStates;
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
                file >> frItem;
                file >> toStateName;
                file >> cond;
                if (frItem == "ToState")
                {
                    mState.toStates.emplace(toStateName, (LilStateChangeConditions)cond);
                    StateObjects[name].AddNewState(mState);
                }
            }

        }
    }
}

void StaticDataManager::LoadAnimation(std::ifstream& file)
{
    std::string name, repType, hasSpline, splineRot;
    RepeatType repeatType = REPEAT_STOP;
    int numKeyFrames;
    file >> name;
    file >> numKeyFrames;
    file >> repType;
    file >> hasSpline;
    file >> splineRot;
    if (repType == "LOOP")
    {
        repeatType = REPEAT_LOOP;
    } else if (repType == "CLAMP")
    {
        repeatType = REPEAT_CLAMP;
    }
    Animations.emplace(name, Animation(repeatType));

    if (hasSpline == "true")
    {
        Animations[name].spline = LilSpline(numKeyFrames);
        Animations[name].followSpline = true;
        if (splineRot == "true")
        {
            Animations[name].rotSpline = true;
        }
    }

    std::vector<glm::vec2> positions;
    for (int i = 0; i < numKeyFrames; i++)
    {
        std::string kFrameCheck;
        file >> kFrameCheck;
        if (kFrameCheck == "KeyFrame")
        {
            float time;
            int numParams;
            file >> time;
            file >> numParams;
            Animations[name].insertKeyFrame({time});
            for (int j = 0; j < numParams; j++)
            {
                std::string paramType;
                file >> paramType;
                KeyFrame& keyFrame = Animations[name].getKeyFrame(i);
                if (paramType == "Sprite")
                {
                    std::string spriteImage;
                    int frame;
                    float offX, offY, szX, szY;
                    file >> spriteImage;
                    file >> frame;
                    file >> offX;
                    file >> offY;
                    file >> szX;
                    file >> szY;
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
                    float xPos, yPos, zPos, rot, xScale, yScale;
                    file >> xPos;
                    file >> yPos;
                    file >> rot;
                    file >> xScale;
                    file >> yScale;
                    keyFrame.hasTransformData = true;
                    keyFrame.ftd.objPos = {xPos, yPos};
                    keyFrame.ftd.objRot = rot;
                    keyFrame.ftd.objScale = {xScale, yScale};
                }
            }
        }
        if (hasSpline == "true")
        {
            std::string splCheck;
            file >> splCheck;
            if (splCheck == "SplinePoint")
            {
                float xPos, yPos;
                file >> xPos;
                file >> yPos;
                positions.emplace_back(xPos, yPos);
            }
        }
    }
    if (hasSpline == "true")
    {
        Animations[name].spline.initPoints(positions);
    }
}

void StaticDataManager::LoadSpriteInfo(std::ifstream& file)
{
    std::string name, importType;
    file >> name;
    file >> importType;
    SpriteInfo.emplace(name, TexImportData());

    if (importType == "Auto")
    {
        SpriteInfo[name].spriteType = SPR_AUTO;
        SpriteInfo[name].numSprites = 1;
    } else if (importType == "Uniform")
    {
        int numSprites, width, height;
        file >> numSprites;
        file >> width;
        file >> height;
        SpriteInfo[name].spriteType = SPR_UNIFORM;
        SpriteInfo[name].numSprites = numSprites;
        SpriteInfo[name].width = width;
        SpriteInfo[name].height = height;
    } else if (importType == "Manual")
    {
        std::cout << "Manual size spritesheet not implemented yet, ending load";
    } else if (importType == "Cursor")
    {
        SpriteInfo[name].spriteType = SPR_CURSOR;
    } else if (importType == "WindowImage")
    {
        SpriteInfo[name].spriteType = SPR_WINDOW_IMAGE;
    }
}