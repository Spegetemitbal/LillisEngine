#pragma once

#include "Behaviors/BehaviorSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "Physics/PhysicsSystem.h"


class GameObject;

class GameObjectManager
{
public:
	static GameObjectManager* world;

	static void DelWorld() { delete world; }

	void clearAll();

	GameObjectManager() {}
	~GameObjectManager() {};

	int numObjects = 0;

	//GameObject* findObject(GameObject g);

	GameObject* addObject();
	/* void addCollider(int objNum, float w, float h);
	void addRenderer(int objNum, Color c, float w, float h);
	void addPlayer(int objNum);
	void addColorChanger(int objNum);
	void addRotator(int objNum, double angle);

	void addCollider(float w, float h);
	void addRenderer(Color c, float w, float h);
	void addPlayer();
	void addColorChanger();
	void addRotator(double angle); */

	std::vector<GameObject> objects = std::vector<GameObject>();
	std::vector<RectangleCollider> colliders = std::vector<RectangleCollider>();
	std::vector<RectangleRenderer> renderers = std::vector<RectangleRenderer>();
	std::vector<PlayerController> players = std::vector<PlayerController>();
	std::vector<Rotator> rotators = std::vector<Rotator>();
};

//Note that INTS in C++ can hold 4 characters instead of a number, pretty sweet.