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


	Transform thisTransform = thisObject->transform;
	Transform thatTransform = other.thisObject->transform;

	if (abs(thisTransform.x - thatTransform.x) < thisWidth + otherWidth && abs(thisTransform.y - thatTransform.y) < thisHeight + otherHeight)
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