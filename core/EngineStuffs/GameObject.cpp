#include "GameObject.h"
#include "GameObjectManager.h"

#define WORLD GameObjectManager::world

void GameObject::SetSprite(std::string name)
{
	sprite = name;
}

void GameObject::SetActive(bool active)
{
	isActive = active;
	if (!isActive)
	{
		isActive = false;
		transform = Transform();
		//sprite = nullptr;
		collider = nullptr;
		player = nullptr;
		rotator = nullptr;
	} else
	{
		isEnabled = true;
	}
}

RectangleCollider* GameObject::CreateCollider(float w, float h, int id)
{
	if (collider == nullptr)
	{
		collider = WORLD->addCollider(w, h, id);
		collider->setControlledObject(this);
	}
	return collider;
}

//[Insert steve vai song title here] please make sure this isn't a copy or implicit constructor. plz.
PlayerController* GameObject::CreatePlayerController()
{
	if (player == nullptr)
	{
		player = WORLD->addPC();
		player->setControlledObject(this);
	}
	return player;
}

Rotator* GameObject::CreateRotator(double angle)
{
	if (rotator == nullptr)
	{
		rotator = WORLD->addRot(angle);
		rotator->setControlledObject(this);
	}
	return rotator;
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