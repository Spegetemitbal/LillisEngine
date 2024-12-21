#include "SceneLoader.h"
#include "../EngineStuffs/GameObject.h"
#include "../EngineStuffs/GameObjectManager.h"
#include "../EngineStuffs/MemoryManagement/Serializer.h"
#include "../EngineStuffs/Behaviors/BehaviorSystem.h"
#include "ResourceManager.h"

#include <cctype>

#define WORLD GameObjectManager::world

const int MAX_PARAMS = 10;

SceneLoader::SceneLoader()
{
	//Map static componentCreate functions here.
	//1347175252
	//compMap['PLCT'] = PlayerController::CreatePlayerController;
	//1380144195
	compMap['RCTC'] = RectangleCollider::CreateRectangleCollider;
	//1380144210
	//compMap['RCTR'] = RectangleRenderer::CreateRectangleRenderer;
	//1380930642
	//compMap['ROTR'] = Rotator::CreateRotator;
}

SceneLoader::~SceneLoader()
{
	compMap.clear();
}

void SceneLoader::LoadData(const std::string& fileName)
{
	if (ResourceManager::DataFiles.find(fileName) == ResourceManager::DataFiles.end())
	{
		std::cout << "File " << fileName << " does not exist!" << '\n';
		return;
	}
	FileDataWrapper data = ResourceManager::DataFiles[fileName];
	std::ifstream stream;
	stream.open(data.getFilePath());

	Serializer ser = Serializer();

	if (!stream.is_open())
	{
		return;
	}

	std::string word;
	while (stream.good())
	{
		stream >> word;
		if (word == "Object")
		{
			float x, y;
			stream >> x;
			stream >> y;

			//Intake texture ID here

			//std::cout << word << " " << x << " " << y << " " << std::endl;

			//stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			std::string component;
			LilObj<GameObject> G = WORLD->addObject();
			G->transform.x = x;
			G->transform.y = y;
			G->transform.z = 0;

			//1162757196
			while (component != ";")
			{
				stream >> component;
				if (component != ";")
				{
					if (component == "Collider")
					{
						float width, height;
						int tag;
						stream >> width;
						stream >> height;
						stream >> tag;
						G->CreateCollider(width, height, tag);
					}
					if (component == "Sprite")
					{
						std::string spriteName;
						stream >> spriteName;
						G->SetSprite(spriteName);
					}
					if (component == "Behavior")
					{
						//std::stringstream ss;
						std::string currentParam;
						std::string behvName;
						stream >> behvName;

						LilObj<Behavior> behv = G->CreateBehaviorGeneric(behvName);

						stream >> currentParam;
						while (currentParam != ")")
						{
							if (currentParam != ")")
							{
								char fin = currentParam.back();
								char prev = currentParam[currentParam.size() - 2];
								if (std::isdigit(prev))
								{
									if (fin == 'd')
									{
										currentParam.pop_back();
										double d = stod(currentParam);
										ser.InsertToBuffer<double>(&d);
									} else if (fin == 'f')
									{
										currentParam.pop_back();
										float f = stof(currentParam);
										ser.InsertToBuffer<float>(&f);
									} else if (fin == 'i')
									{
										currentParam.pop_back();
										int i = stoi(currentParam);
										ser.InsertToBuffer<int>(&i);
									} else
									{
										ser.InsertToBuffer(currentParam.c_str());
									}
								} else
								{
									ser.InsertToBuffer(currentParam.c_str());
								}
								//std::cout << currentParam << " ";
							}
							currentParam.clear();
							stream >> currentParam;
						}
						behv->Deserialize(ser);
					}
				}
			}
			component.clear();
			//std::cout << component << std::endl;
		} else if (word == "END")
		{
			break;
		}
	}

	stream.close();
}

SceneLoader* SceneLoader::SCL = nullptr;