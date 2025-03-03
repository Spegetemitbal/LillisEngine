//
// Created by Somed on 1/27/2025.
//

#include "Prototype.h"

#include "StaticDataManager.h"
#include "EngineStuffs/GameObject.h"
#include "EngineStuffs/GameObjectManager.h"

std::unordered_map<std::string, unsigned int> Prototype::numExisting = std::unordered_map<std::string, unsigned int>();

#ifndef WORLD
#define WORLD GameObjectManager::world
#endif

LilObj<GameObject> Prototype::CreatePrototype(const std::string &name, glm::vec2 pos, float rot)
{
    FileDataWrapper data = ResourceManager::DataFiles[name];
    if (data.getFileType() != "prototype")
    {
        std::cout << name << " is not a prototype" << std::endl;
        return {};
    }

    if (numExisting.contains(name))
    {
        numExisting[name]++;
    } else
    {
        numExisting.emplace(name, 0);
    }

    std::ifstream stream;
	stream.open(data.getFilePath());

	Serializer ser = Serializer();

	if (!stream.is_open())
	{
		return {};
	}

	LilObj<GameObject> root;

	std::string word;
	while (stream.good())
	{
		stream >> word;
		if (word == "Object")
		{
			float x, y, r;
			std::string name, parent;
			stream >> x;
			stream >> y;
			stream >> r;
			stream >> name;
			stream >> parent;

			if (numExisting[name] > 0)
			{
				name += (char)numExisting[name];
			}

			LilObj<GameObject> G = WORLD->addObject(x + pos.x, y + pos.y, name);
			G->transform->Rotate(r);
			if (parent != "NONE")
			{
				WORLD->SetObjectParent(parent, G);
			} else if (root.Exists())
			{
				std::cout << "Only one root object per prototype!" << '\n';
				break;
			} else
			{
				root = G;
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
		}
	}

	stream.close();
	return root;
}

