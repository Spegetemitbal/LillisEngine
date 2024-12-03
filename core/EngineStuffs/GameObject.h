#pragma once
#include "Transform.h"
#include "Behaviors/BehaviorSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Graphics/GraphicsSystem.h"

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

	//Inactive objects are wiped.
	void SetActive(bool active);
	static void Destroy(GameObject* object) {object->SetActive(false);};
	bool GetActive() const { return isActive; };

	//Whether the object is turned on or not
	bool isEnabled = false;

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

	bool isActive = false;
	int entityID = 0;
};