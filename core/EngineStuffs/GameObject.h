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
	GameObject(float x, float y, const std::string &name, std::string parent)
	{
		transform = Transform(x, y);
		objName = name;
		entityID = nextID;
		nextID++;
	};

	~GameObject()
	= default;

	//Inactive objects are wiped.
	void SetActive(bool active);
	bool GetActive() const { return isActive; };

	void SetEnabled(bool enabled) {if (isActive) {isEnabled = enabled;} };
	bool GetEnabled() const { return isEnabled; };

	//For cache efficiency, 
	//std::vector<Behavior> behaviors = std::vector<Behavior>();

	void SetSpriteImage(const std::string &name);
	LilObj<Sprite> CreateSprite(const std::string &image);
	LilObj<RectangleCollider> CreateCollider(float w, float h, int id);

	unsigned int GetID() const {return entityID;};
	std::string GetName() const {return objName;};

	size_t SerializeTransform(char* buffer, size_t bufSize);
	size_t DeSerializeTransform(char* buffer, size_t bufSize);

	Transform transform{};

	//Behavior* getBehavior(LILLIS::string name);

	LilObj<RectangleCollider> getCollider() {return collider;};
	LilObj<Sprite> getSprite() { return sprite; }

	LilObj<GameObject> thisObject;

	template<typename Beh>
	LilObj<Beh> GetBehavior(const std::string& name)
	{
		if (behaviorMap.find(name) != behaviorMap.end())
		{
			return {behaviorMap[name].GetPool(), entityID};
		} else
		{
			return {};
		}
	}

	//For use by Sceneloader, technically won't break if a user uses it?
	LilObj<Behavior> CreateBehaviorGeneric(const std::string& name);

	template<typename Beh>
	LilObj<Beh> CreateBehavior(const std::string& name)
	{
		LilObj<Behavior> b = CreateBehaviorGeneric(name);
		return {b.GetPool(), b.GetID()};
	}

protected:
	std::string objName;
	LilObj<Sprite> sprite;
	LilObj<RectangleCollider> collider;
	unordered_map<std::string, LilObj<Behavior>> behaviorMap;

	friend class GameObjPool;

	//Whether the object is turned on or not
	bool isEnabled = false;
	//Whether the object is garbage or not.
	bool isActive = false;
	unsigned int entityID = 0;
	static unsigned int nextID;
};