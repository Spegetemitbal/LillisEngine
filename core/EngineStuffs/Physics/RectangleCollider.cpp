#include "../GameObject.h"
#include "RectangleCollider.h"
#include "Utils/Events/CollisionEvent.h"
//#include <cstdlib>

Component* RectangleCollider::CreateRectangleCollider(GameObject* G, int* params)
{
	//Probs fix this lol.
	return nullptr; //G->CreateCollider(params[0], params[1], params[2]);
}

bool RectangleCollider::CheckCollision(RectangleCollider other)
{
	float otherWidth = other.colliderWidth / 2;
	float otherHeight = other.colliderHeight / 2;
	float thisWidth = colliderWidth / 2;
	float thisHeight = colliderHeight / 2;


	LilObj<Transform> thisTransform = thisObject->transform;
	LilObj<Transform> thatTransform = other.thisObject->transform;

	if (abs(thisTransform->GlobalPosition().x - thatTransform->GlobalPosition().x)
		< thisWidth + otherWidth && abs(thisTransform->GlobalPosition().y - thatTransform->GlobalPosition().y) < thisHeight + otherHeight)
	{
		EventSystem* e = EventSystem::getInstance();
		e->fireEvent(CollisionEvent(tag, other.tag));
		return true;
	}
	else
	{
		return false;
	}
}