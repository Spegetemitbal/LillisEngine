#include "SceneLoader.h"
#include "../EngineStuffs/GameObject.h"
#include "../EngineStuffs/GameObjectManager.h"
#include "../EngineStuffs/MemoryManagement/Serializer.h"
#include "../EngineStuffs/Behaviors/BehaviorSystem.h"
#include "ResourceManager.h"

#include <cctype>

#include "Prototype.h"
#include "StaticDataManager.h"
#include "EngineStuffs/UI/UISystem.h"
#include "EngineStuffs/Tilemaps/TileMap.h"

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
			float x, y, r;
			std::string name, parent;
			stream >> x;
			stream >> y;
			stream >> r;
			stream >> name;
			stream >> parent;

			//Intake texture ID here
			LilObj<GameObject> G = WORLD->addObject(x, y, name);
			G->transform->Rotate(r);
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
					if (component == "RigidBody")
					{
						std::string shape, bodyType;
						RigidBodyType rbType;
						RigidBodyShape rbShape;
						float mass, density;
						PhysicsMaterial physMaterial;
						BoxData boxData;
						CircleData circleData;

						stream >> shape;
						stream >> bodyType;
						stream >> mass;
						stream >> density;
						stream >> physMaterial.restitution;
						stream >> physMaterial.staticFriction;
						stream >> physMaterial.dynamicFriction;
						if (shape == "Box")
						{
							stream >> boxData.boxSize.x;
							stream >> boxData.boxSize.y;
							rbShape = RigidBodyShape::RB_BOX;
						} else if (shape == "Circle")
						{
							stream >> circleData.radius;
							rbShape = RigidBodyShape::RB_CIRCLE;
						} else
						{
							std::cout << "Other shapes not implemented yet" << std::endl;
						}

						if (bodyType == "Static")
						{
							rbType = RigidBodyType::RB_STATIC;
						} else if (bodyType == "Kinematic")
						{
							rbType = RigidBodyType::RB_KINEMATIC;
						} else
						{
							rbType = RigidBodyType::RB_DYNAMIC;
						}

						G->CreateRigidBody(rbShape, rbType, mass, density, physMaterial, boxData, circleData);
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
							s->setRenderSize(tex.spriteSizes[0]);
						} else
						{
							s->setRenderSize({sprWidth, sprHeight});
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
		} else if (word == "UIObject")
		{
			std::string name, image;
			unsigned int layer, frame;
			float renderPosX, renderPosY, screenPosX, screenPosY;
			UISystem::getInstance()->addUIObject(name, image, frame, layer, {screenPosX, screenPosY},
				{renderPosX, renderPosY});
		} else if (word == "Grid")
		{
			std::string shape;
			float x, y;

			GridShape gridShape = GRID_RECTANGULAR;
			stream >> shape;
			if (shape == "Isometric")
			{
				gridShape = GRID_ISOMETRIC;
			} else if (shape == "Hexagonal")
			{
				gridShape = GRID_HEXAGON;
			}

			stream >> x;
			stream >> y;

			WORLD->createTileGrid(gridShape, {x,y});
		} else if (word == "TileMap")
		{
			TileGrid* tileGrid = WORLD->getTileGrid();
			if (tileGrid == nullptr)
			{
				std::cerr << "Tile Grid does not exist to put on tilemap" << std::endl;
				throw;
			}

			int width, height, gridX, gridY;
			char input;
			std::string tileSetName;

			stream >> tileSetName;
			stream >> gridX;
			stream >> gridY;
			stream >> width;
			stream >> height;

			TileSet set = StaticDataManager::TileSets[tileSetName];

			TileMap* map = WORLD->createTileMap(set,
				{gridX,gridY}, {width,height});

			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					stream >> input;
					map->setTile({x,y}, input);
				}
			}
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
