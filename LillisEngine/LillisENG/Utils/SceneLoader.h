#pragma once
#include <iostream>
#include <fstream>
#include "../EngineStuffs/Component.h"
#include <unordered_map>

typedef Component* (*componentFunction)(GameObject* G, int* param);

class SceneLoader
{
public:
	static SceneLoader* SCL;

	SceneLoader();
	~SceneLoader();

	void LoadData(std::string fileName);

private:
	std::unordered_map<int, componentFunction> compMap;
};