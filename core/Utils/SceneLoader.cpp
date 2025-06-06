#include "SceneLoader.h"
#include "../EngineStuffs/GameObject.h"
#include "../EngineStuffs/WorldManager.h"
#include "../EngineStuffs/MemoryManagement/Serializer.h"
#include "../EngineStuffs/Behaviors/BehaviorSystem.h"
#include "ResourceManager.h"

#include <cctype>

#include "Prototype.h"
#include "StaticDataManager.h"
#include "EngineStuffs/ObjectGrouping.h"
#include "EngineStuffs/UI/UISystem.h"
#include "EngineStuffs/Tilemaps/TileMap.h"

#ifndef WORLD
#define WORLD WorldManager::getInstance()->GetCurrentWorld()
#endif

void SceneLoader::LoadData(const std::string& fileName)
{
	if (WORLD == nullptr)
	{
		std::cout << "Invalid LoadData Call" << std::endl;
		return;
	}

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
			std::string name, transformparent, parent;
			stream >> x;
			stream >> y;
			stream >> r;
			stream >> name;
			stream >> parent;
			stream >> transformparent;

			//Intake texture ID here
			LilObj<GameObject> G = WORLD->addObject(x, y, name);
			G->transform->SetLocalRotation(r, true);
			if (transformparent != "NONE")
			{
				WORLD->SetTransformParent(transformparent, G);
			}
			if (parent != "NONE")
			{
				LilObj<GameObject> par = WORLD->getObjectByName(parent);
				if (par.Exists())
				{
					WORLD->objectGrouping()->SetParent(par, G);
				} else
				{
					std::cout << "Object " << parent << " does not exist" << std::endl;
				}
			}

			std::string component;

			//1162757196
			while (component != ";")
			{
				stream >> component;
				if (component != ";")
				{
					if (component == "RigidBody")
					{
						int tag = 0;
						std::string shape, bodyType, trigger;
						RigidBodyType rbType;
						RigidBodyShape rbShape;
						float mass, density;
						PhysicsMaterial physMaterial;
						BoxData boxData;
						CircleData circleData;
						bool isTrigger = false;

						stream >> tag;
						stream >> shape;
						stream >> bodyType;
						stream >> trigger;
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

						if (trigger == "Trigger")
						{
							isTrigger = true;
						}

						G->CreateRigidBody(tag, rbShape, rbType, isTrigger, mass, density, physMaterial, boxData, circleData);
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
							G->CreateSingleAnimator(&StaticDataManager::Animations[animName], false);
						} else if (animType == "Multiple")
						{
							//Assume multiple
							std::string stateObjName;
							stream >> stateObjName;
							G->CreateAnimator(&StaticDataManager::StateObjects[stateObjName], false);
							//std::cout << "State based animation not implemented" << '\n';
						}
					}
					if (component == "Behavior")
					{
						//std::stringstream ss;
						std::string currentParam;
						std::string behvName;
						stream >> behvName;

						LilObj<Behavior> behv = G->CreateBehaviorGeneric(behvName, false);

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
					if (component == "ParticleEmitter")
					{
						std::string particleName, inheritsTransformStr;
						bool inheritsTransform = true;
						unsigned int maxParticles, layer;
						ParticleEmitterData particleData = ParticleEmitterData();
						stream >> particleName;
						if (!StaticDataManager::ParticleEffects.contains(particleName))
						{
							std::cout << "Particle: " << particleName << " Does not exist, terminating load." << std::endl;
							break;
						}
						stream >> layer;
						stream >> maxParticles;

						stream >> inheritsTransformStr;
						if (inheritsTransformStr != "INHERIT")
						{
							stream >> particleData.startPos.x;
							stream >> particleData.startPos.y;
							inheritsTransform = false;
						}

						stream >> particleData.spawnBounds.x;
						stream >> particleData.spawnBounds.y;
						stream >> particleData.minInitVelocity.x;
						stream >> particleData.minInitVelocity.y;
						stream >> particleData.maxInitVelocity.x;
						stream >> particleData.maxInitVelocity.y;
						stream >> particleData.startAcceleration.x;
						stream >> particleData.startAcceleration.y;
						stream >> particleData.minStartScale.x;
						stream >> particleData.minStartScale.y;
						stream >> particleData.maxStartScale.x;
						stream >> particleData.maxStartScale.y;
						stream >> particleData.startColor.r;
						stream >> particleData.startColor.g;
						stream >> particleData.startColor.b;
						stream >> particleData.startColor.a;
						stream >> particleData.endColor.r;
						stream >> particleData.endColor.g;
						stream >> particleData.endColor.b;
						stream >> particleData.endColor.a;
						stream >> particleData.lifetime;
						stream >> particleData.spawnSpeed;

						LilObj<ParticleEmitter> pe = G->CreateParticleEmitter(particleData, inheritsTransform);
						pe->layer = layer;
						pe->SetEffect(&StaticDataManager::ParticleEffects[particleName]);
						pe->SetMaxParticles(maxParticles);
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
		} else if (word == "PhysicsSettings")
		{
			PhysicsSettings settings = PhysicsSettings::PhysicsSettings();
			float gravX, gravY;
			unsigned int iters;
			stream >> gravX;
			stream >> gravY;
			stream >> iters;

			settings.gravity = {gravX, gravY};
			settings.physicsIterations = iters;

			PhysicsSystem::getInstance()->SetPhysicsSettings(settings);
		} else if (word == "RenderSettings")
		{
			//TODO implement data driven render settings
			unsigned int resolutionWidth, resolutionHeight, windowWidth, windowHeight, pixelsPerUnit;
			std::string fullScreen;
			stream >> resolutionWidth;
			stream >> resolutionHeight;
			stream >> windowWidth;
			stream >> windowHeight;
			stream >> pixelsPerUnit;
			stream >> fullScreen;
			if (fullScreen == "fullscreen")
			{
				RenderSettings settings = RenderSettings(resolutionWidth, resolutionHeight, windowWidth, windowHeight, pixelsPerUnit, true);
				//GraphicsSystem::getInstance()->
			} else
			{
				RenderSettings settings = RenderSettings(resolutionWidth, resolutionHeight, windowWidth, windowHeight, pixelsPerUnit, false);
			}
		}
	}
	WORLD->initializeAllComponents();
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
