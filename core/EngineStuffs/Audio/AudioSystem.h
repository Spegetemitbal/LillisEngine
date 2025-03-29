#pragma once

#include "Utils/FileDataWrapper.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

class AudioSystem
{
public:

    static AudioSystem* createInstance();
    static AudioSystem* getInstance();
    static void destroyInstance();

    bool Init();

    void SetMasterVolume(float volume);

    void LoadSound(FileDataWrapper sound);

    void PlaySound(const std::string& sound, bool looping,u64 frame = 0);
    bool isAudioPlaying(const std::string& sound);
    //void PauseSound(std::string sound);
    void StopSound(const std::string& sound);

private:

    static AudioSystem* instance;
    AudioSystem();
    ~AudioSystem();

    bool initted = false;

    std::unordered_map<std::string, ma_sound> sounds;

    ma_engine* soundEngine;
};