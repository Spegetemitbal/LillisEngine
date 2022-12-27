#pragma once
#include "GameObject.h"
#include <vector>

const int MAX_OBJECTS = 50;

struct GameObjectManager
{
	static GameObjectManager* world;

	static void DelWorld() { delete world; }

	GameObjectManager() {}
	~GameObjectManager() {};

	int numObjects = 0;
	int numColliders = 0;
	int numRenderers = 0;
	int numColorChangers = 0;
	int numPlayers = 0;
	int numRotators = 0;

	//GameObject* findObject(GameObject g);

	GameObject* addObject();
	void addCollider(int objNum, float w, float h);
	void addRenderer(int objNum, Color c, float w, float h);
	void addPlayer(int objNum);
	void addColorChanger(int objNum);
	void addRotator(int objNum, double angle);

	void addCollider(float w, float h);
	void addRenderer(Color c, float w, float h);
	void addPlayer();
	void addColorChanger();
	void addRotator(double angle);

	GameObject objects[MAX_OBJECTS];
	RectangleCollider colliders[MAX_OBJECTS];
	RectangleRenderer renderers[MAX_OBJECTS];
	PlayerController players[MAX_OBJECTS];
	Rotator rotators[MAX_OBJECTS];
};

//Note that INTS in C++ can hold 4 characters instead of a number, pretty sweet.