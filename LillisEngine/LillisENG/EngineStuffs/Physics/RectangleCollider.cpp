#include "../GameObject.h"
#include "RectangleCollider.h"
#include <cstdlib>

Component* RectangleCollider::CreateRectangleCollider(GameObject* G, int* params)
{
	return G->CreateCollider(params[0], params[1]);
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
		return true;
	}
	else
	{
		return false;
	}
}