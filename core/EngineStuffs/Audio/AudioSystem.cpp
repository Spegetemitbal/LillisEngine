//
// Created by Somed on 1/28/2025.
//

#include "AudioSystem.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "Utils/ResourceManager.h"

AudioSystem* AudioSystem::instance = nullptr;

AudioSystem *AudioSystem::createInstance()
{
    if (instance == nullptr)
    {
        instance = DBG_NEW AudioSystem();
    }
    return instance;
}


AudioSystem* AudioSystem::getInstance()
{
    if (instance == nullptr)
    {
        instance = createInstance();
    }
    return instance;
}

void AudioSystem::destroyInstance()
{
    if (instance != nullptr)
    {
        delete instance;
    }
}


bool AudioSystem::Init()
{
    ma_result result;

    result = ma_engine_init(nullptr, soundEngine);
    if (result != MA_SUCCESS) {
        return false;  // Failed to initialize the engine.
    }

    for (auto f : ResourceManager::DataFiles)
    {
        if (f.second.getFileType() == "sound")
        {
            LoadSound(f.second);
        }
    }

    initted = true;
    return true;
}

AudioSystem::AudioSystem()
{
    soundEngine = (ma_engine*)malloc(sizeof(*soundEngine));
}

AudioSystem::~AudioSystem()
{
    for (auto s : sounds)
    {
        ma_sound_uninit(&s.second);
    }
    ma_engine_uninit(soundEngine);
    free(soundEngine);
}

void AudioSystem::SetMasterVolume(float volume)
{
    if (!initted)
    {
        std::cerr << "Audio engine not initted!" << std::endl;
        return;
    }

    if (volume > 1.0f)
    {
        ma_engine_set_volume(soundEngine, 1.0f);
    } else if (volume < 0.0f)
    {
        ma_engine_set_volume(soundEngine, 0.0f);
    } else
    {
        ma_engine_set_volume(soundEngine, volume);
    }
}

void AudioSystem::LoadSound(FileDataWrapper sound)
{
    if (!initted)
    {
        std::cerr << "Audio engine not initted!" << std::endl;
        return;
    }

    if (sounds.contains(sound.getFileName()))
    {
        std::cout << "Sound already exists" << std::endl;
        return;
    }

    if (sound.getFileType() != "sound")
    {
        std::cout << "Not a sound file" << std::endl;
        return;
    }

    sounds.emplace(sound.getFileName(), ma_sound());
    ma_result result;

    result = ma_sound_init_from_file(
        soundEngine, sound.getFilePath().c_str(), 0, NULL, NULL, &sounds[sound.getFileName()]);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to load sound: " << sound.getFileName() << std::endl;
        sounds.erase(sound.getFileName());
        return;
    }

}


void AudioSystem::PlaySound(const std::string& sound, bool looping, u64 frame)
{
    if (!initted)
    {
        std::cerr << "Audio engine not initted!" << std::endl;
        return;
    }

    if (!sounds.contains(sound))
    {
        std::cerr << "Sound: " << sound << " Not found." << std::endl;
        return;
    }

    if (looping)
    {
        ma_sound_set_looping(&sounds[sound], true);
    } else
    {
        ma_sound_set_looping(&sounds[sound], false);
    }


    if (!ma_sound_is_playing(&sounds[sound]))
    {
        //May be unsafe! cap to max.
        ma_sound_seek_to_pcm_frame(&sounds[sound], frame);
        ma_sound_start(&sounds[sound]);
    } else
    {
        std::cout << "Sound: " << sound << " already playing" << std::endl;
    }
}


void AudioSystem::StopSound(const std::string& sound)
{
    if (!initted)
    {
        std::cerr << "Audio engine not initted!" << std::endl;
        return;
    }

    if (!sounds.contains(sound))
    {
        std::cerr << "Sound: " << sound << " Not found." << std::endl;
        return;
    }

    ma_sound_stop(&sounds[sound]);
}


bool AudioSystem::isAudioPlaying(const std::string& sound)
{
    if (!initted)
    {
        std::cerr << "Audio engine not initted!" << std::endl;
        return false;
    }

    if (!sounds.contains(sound))
    {
        std::cerr << "Sound: " << sound << " Not found." << std::endl;
        return false;
    }

    return ma_sound_is_playing(&sounds[sound]);
}






