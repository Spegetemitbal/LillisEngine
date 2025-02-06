#include "SceneLoader.h"
#include "../EngineStuffs/GameObject.h"
#include "../EngineStuffs/GameObjectManager.h"
#include "../EngineStuffs/MemoryManagement/Serializer.h"
#include "../EngineStuffs/Behaviors/BehaviorSystem.h"
#include "ResourceManager.h"

#include <cctype>

#include "Prototype.h"
#include "StaticDataManager.h"

#ifndef WORLD
#define WORLD GameObjectManager::world
#endif

void SceneLoader::LoadData(const std::string& fileName)
{
	SceneInfo info = AnalyzeScene(fileName);
	if (info.numObjects == 0)
	{
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
			std::string name, parent;
			stream >> x;
			stream >> y;
			stream >> name;
			stream >> parent;

			//Intake texture ID here
			LilObj<GameObject> G = WORLD->addObject(x, y, name);
			if (parent != "NONE")
			{
				WORLD->SetObjectParent(parent, G);
			}

			std::string component;

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
						unsigned int lyr, sprWidth, sprHeight;
						stream >> spriteName;
						stream >> lyr;
						LilObj<Sprite> s = G->CreateSprite(spriteName, lyr);
						stream >> sprWidth;
						stream >> sprHeight;
						//s->SetLayer(lyr);
						s->frame = 0;
						if (sprWidth == 0 && sprHeight == 0)
						{
							//Add import settings to determine default size later.
							Texture2D tex = ResourceManager::GetTexture(s->image);
							s->renderSize = tex.spriteSizes[0];
						} else
						{
							s->renderSize = {sprWidth, sprHeight};
						}
					}
					if (component == "Animator")
					{
						std::string animType;
						stream >> animType;
						if (animType == "Single")
						{
							std::string animName;
							stream >> animName;
							G->CreateSingleAnimator(&StaticDataManager::Animations[animName]);
						} else if (animType == "Multiple")
						{
							//Assume multiple
							std::string stateObjName;
							stream >> stateObjName;
							G->CreateAnimator(&StaticDataManager::StateObjects[stateObjName]);
							//std::cout << "State based animation not implemented" << '\n';
						}
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
		} else if (word == "Prototype")
		{
			std::string name;
			float x, y, rot;
			stream >> name;
			stream >> x;
			stream >> y;
			stream >> rot;
			Prototype::CreatePrototype(name, {x,y}, rot);
		}
	}

	stream.close();
}

SceneInfo SceneLoader::AnalyzeScene(const std::string &fileName)
{
	SceneInfo info = SceneInfo();

	if (ResourceManager::DataFiles.find(fileName) == ResourceManager::DataFiles.end())
	{
		std::cout << "File " << fileName << " does not exist!" << '\n';
		return {};
	}
	FileDataWrapper data = ResourceManager::DataFiles[fileName];
	std::ifstream stream;
	stream.open(data.getFilePath());

	Serializer ser = Serializer();

	if (!stream.is_open())
	{
		return {};
	}

	std::string word;
	while (stream.good())
	{
		stream >> word;
		if (word == "Object")
		{
			info.numObjects++;
		}
	}

	stream.close();
	return info;
}
