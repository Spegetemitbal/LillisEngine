//
// Created by Somed on 12/2/2024.
//

#include "FileDataWrapper.h"
#include <filesystem>

FileDataWrapper::FileDataWrapper(const char *file)
{
    std::filesystem::path path(file);
    filePath = path.string();
    fileName = path.stem().string();
    std::string tempFileType = path.extension().string();
    if (tempFileType == ".lvl")
    {
        fileType = "level";
    } else if (tempFileType == ".sav")
    {
        fileType = "save";
    } else if (tempFileType == ".lil")
    {
        fileType = "settings";
    } else
    {
        std::cout << "Unrecognized file type: " << tempFileType << std::endl;
        fileType = tempFileType;
    }
}
