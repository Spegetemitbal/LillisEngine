#include "SceneLoader.h"
#include <string>
#include "../EngineStuffs/GameObject.h"

#define WORLD GameObjectManager::world

const int MAX_PARAMS = 10;

SceneLoader::SceneLoader()
{
	//Map static componentCreate functions here.
	//1347175252
	compMap['PLCT'] = PlayerController::CreatePlayerController;
	//1380144195
	compMap['RCTC'] = RectangleCollider::CreateRectangleCollider;
	//1380144210
	compMap['RCTR'] = RectangleRenderer::CreateRectangleRenderer;
	//1380930642
	compMap['ROTR'] = Rotator::CreateRotator;
}

SceneLoader::~SceneLoader()
{
	compMap.clear();
}

void SceneLoader::LoadData(std::string fileName)
{
	std::ifstream stream;
	stream.open(fileName);

	if (!stream.is_open())
	{
		return;
	}

	std::string word;
	while (stream.good())
	{
		stream >> word;
		if (word == "Obj")
		{
			int x, y;
			stream >> x;
			stream >> y;
			std::cout << word << " " << x << " " << y << " " << std::endl;

			stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			int component = 0;
			int currentParam = 0;
			GameObject* G = WORLD->addObject();
			G->transform.x = x;
			G->transform.y = y;
			G->transform.z = 0;

			//1162757196
			while (component != 'ENDL')
			{
				stream >> component;
				currentParam = 0;
				if (component != 'ENDL')
				{
					std::cout << component << " ";
					int componentParam[MAX_PARAMS];
					//1162757200
					for (int i = 0; currentParam != 'ENDP'; i++)
					{
						stream >> currentParam;
						if (currentParam != 'ENDP')
						{
							componentParam[i] = currentParam;
							std::cout << currentParam << " ";
						}
					}
					compMap[component](G, componentParam);
					std::cout << currentParam << std::endl;
				}
			}
			std::cout << component << std::endl;

		}
	}

	stream.close();
}

SceneLoader* SceneLoader::SCL = nullptr;