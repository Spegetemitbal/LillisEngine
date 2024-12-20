#pragma once
#include "../EngineStuffs/Component.h"

typedef Component* (*componentFunction)(GameObject* G, int* param);

class SceneLoader
{
public:
	static SceneLoader* SCL;

	SceneLoader();
	~SceneLoader();

	static void LoadData(const std::string& fileName);

private:
	std::unordered_map<int, componentFunction> compMap;
};