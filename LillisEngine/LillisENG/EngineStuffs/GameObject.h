#pragma once

#include "GameObjectManager.h"
#include "Transform.h"

class GameObject
{
public:
	//Init all components to nullptr
	GameObject()
	{
		transform = Transform();
		//sprite = nullptr;
		collider = nullptr;
		player = nullptr;
		rotator = nullptr;
	};

	//Probs make these private my guy.
	GameObject(float x, float y, int id)
	{
		transform = Transform();
		transform.x = x;
		transform.y = y;
		entityID = id;
		//sprite = nullptr;
		collider = nullptr;
		player = nullptr;
		rotator = nullptr;
	};

	~GameObject()
	{

	}

	//For cache efficiency, 
	std::vector<Behavior> behaviors = std::vector<Behavior>();

	void SetSprite(std::string name);
	RectangleCollider* CreateCollider(float w, float h, int id);
	PlayerController* CreatePlayerController();
	Rotator* CreateRotator(double angle);

	size_t SerializeTransform(char* buffer, size_t bufSize);
	size_t DeSerializeTransform(char* buffer, size_t bufSize);

	Transform transform;

	//Behavior* getBehavior(LILLIS::string name);

	RectangleCollider* getCollider() { return collider; }
	std::string getSprite() { return sprite; }

protected:
	std::string sprite;
	RectangleCollider* collider = nullptr;
	PlayerController* player = nullptr;
	Rotator* rotator = nullptr;

	int entityID = 0;
};