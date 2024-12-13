#pragma once
#include "Transform.h"
#include "Behaviors/BehaviorSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "MemoryManagement/LilObj.h"

class GameObject
{
public:
	//Init all components to nullptr

	GameObject()
	{
		transform = Transform();
		entityID = nextID;
		nextID++;
	};

	//Probs make these private my guy.
	GameObject(float x, float y)
	{
		transform = Transform();
		transform.x = x;
		transform.y = y;

		entityID = nextID;
		nextID++;
	};

	~GameObject()
	{

	}

	//Inactive objects are wiped.
	void SetActive(bool active);
	bool GetActive() const { return isActive; };

	void SetEnabled(bool enabled) {if (isActive) {isEnabled = enabled;} };
	bool GetEnabled() const { return isEnabled; };

	void SetSprite(std::string name);
	LilObj<RectangleCollider> CreateCollider(float w, float h, int id);
	LilObj<PlayerController> CreatePlayerController();
	LilObj<Rotator> CreateRotator(double angle);

	unsigned int GetID() const {return entityID;};

	size_t SerializeTransform(char* buffer, size_t bufSize);
	size_t DeSerializeTransform(char* buffer, size_t bufSize);

	Transform transform;

	//Behavior* getBehavior(LILLIS::string name);

	LilObj<RectangleCollider> getCollider() { return collider; }
	std::string getSprite() { return sprite; }

	LilObj<GameObject> thisObject;

protected:
	std::string sprite;
	LilObj<RectangleCollider> collider;
	LilObj<PlayerController> player;
	LilObj<Rotator> rotator;

	//Whether the object is turned on or not
	bool isEnabled = false;
	//Whether the object is garbage or not.
	bool isActive = false;
	unsigned int entityID = 0;
	static unsigned int nextID;
};