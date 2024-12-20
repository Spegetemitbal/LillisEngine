#include "GameObject.h"
#include "GameObjectManager.h"

#define WORLD GameObjectManager::world

void GameObject::SetSprite(std::string name)
{
	sprite = name;
}

//Sets object for deletion.
void GameObject::SetActive(bool active)
{
	isActive = active;
	if (!isActive)
	{
		transform = Transform();
		//sprite = nullptr;
		collider = LilObj<RectangleCollider>();
		behaviorMap.clear();
	} else
	{
		isEnabled = true;
	}
}

LilObj<RectangleCollider> GameObject::CreateCollider(float w, float h, int id)
{
	if (!collider.Exists())
	{
		collider = WORLD->addCollider(w, h, id);
		collider->setControlledObject(thisObject);
	}
	return collider;
}

LilObj<Behavior> GameObject::CreateBehaviorGeneric(const std::string& name)
{
	if (behaviorMap.find(name) != behaviorMap.end())
	{
		return behaviorMap[name];
	} else
	{
		LilObj<Behavior> behv = WORLD->addBehavior(name);
		behaviorMap[name] = behv;
		behaviorMap[name]->setControlledObject(thisObject);
		return behv;
	}
}


size_t GameObject::SerializeTransform(char* buffer, size_t bufSize)
{
	size_t bytes_written = 0;
	//bytes_written += copy_to_buffer(&buffer[bytes_written], &entityID, bufSize - bytes_written);
	//bytes_written += copy_to_buffer(&buffer[bytes_written], &transform.x, bufSize - bytes_written);
	//bytes_written += copy_to_buffer(&buffer[bytes_written], &transform.y, bufSize - bytes_written);
	return bytes_written;
}

size_t GameObject::DeSerializeTransform(char* buffer, size_t bufSize)
{
	size_t bytes_read = 4;
	//bytes_read += copy_from_buffer(&buffer[bytes_read], &transform.x, bufSize - bytes_read);
	//bytes_read += copy_from_buffer(&buffer[bytes_read], &transform.y, bufSize - bytes_read);

	return bytes_read;
}

unsigned int GameObject::nextID = 1;