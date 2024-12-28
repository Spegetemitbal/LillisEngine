#pragma once
#include "../EngineStuffs/Component.h"

struct SceneInfo
{
	size_t numObjects = 0;
};

class SceneLoader
{
public:

	SceneLoader() = default;
	~SceneLoader() = default;

	static void LoadData(const std::string& fileName);
	static SceneInfo AnalyzeScene(const std::string& fileName);
};