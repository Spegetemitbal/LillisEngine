#include "GameObject.h"
#include "GameObjectManager.h"

#define WORLD GameObjectManager::world

void GameObject::SetSpriteImage(const std::string &name)
{
	if (sprite.Exists())
	{
		sprite->image = name;
	}
}

//Sets object for deletion.
void GameObject::SetActive(bool active)
{
	isActive = active;
	if (!isActive)
	{
		WORLD->RemoveObjectParent(thisObject, true);
		if (transform.Exists())
		{
			transform->SetLocalPosition({});
			transform->SetLocalRotation(0.0f);
			transform->SetLocalScale({1,1});

			transform->SetActive(false);
		}
		//transform = Transform();
		//sprite = nullptr;
		if (collider.Exists())
		{
			collider->SetActive(false);
		}
		if (sprite.Exists())
		{
			sprite->transform = {};
			sprite->SetActive(false);
		}
		if (animator.Exists())
		{
			animator->SetActive(false);
		}
		for (auto b : behaviorMap)
		{
			if (b.second.Exists())
			{
				b.second->SetActive(false);
			}
		}
		collider = LilObj<RectangleCollider>();
		sprite = LilObj<Sprite>();
		animator = LilObj<Animator>();
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

LilObj<RigidBody> GameObject::CreateRigidBody(RigidBodyShape shape, RigidBodyType rbType, float mass, float density, PhysicsMaterial material, BoxData boxData, CircleData circleData)
{
	if (transform->getIsChild())
	{
		std::cout << "Physics objects cannot be children! " << std::endl;
		return {};
	}

	if (!rigidbody.Exists())
	{
		rigidbody = WORLD->addRigidbody(shape, rbType, mass, density, material, boxData, circleData);
		rigidbody->transform = this->transform;
		rigidbody->setControlledObject(thisObject);
	}
	return rigidbody;
}


LilObj<Sprite> GameObject::CreateSprite(const std::string &image, unsigned int layer)
{
	if (!sprite.Exists())
	{
		sprite = WORLD->addSprite(image, layer);
		sprite->setControlledObject(thisObject);
	}
	return sprite;
}

LilObj<Animator> GameObject::CreateSingleAnimator(Animation *anim)
{
	if (!animator.Exists())
	{
		animator = WORLD->addAnimator();
		animator->setControlledObject(thisObject);
		animator->ConnectComponents();
		animator->SetSingleAnimation(anim);
	}
	return animator;
}

LilObj<Animator> GameObject::CreateAnimator(StateObject *stateObj)
{
	if (!animator.Exists())
	{
		animator = WORLD->addAnimator();
		animator->setControlledObject(thisObject);
		animator->ConnectComponents();
		animator->SetMultiAnimation(stateObj);
	}
	return animator;
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