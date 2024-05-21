#include "pch.h"
#include "Rotator.h"
#include "../GameObject.h"

Component* Rotator::CreateRotator(GameObject* G, int* params)
{
	return G->CreateRotator(params[0]);
}

void Rotator::Update(float deltaTime)
{
	const int DIST = 50;
	myAngle += deltaTime / 2;

	if (myAngle > M_PI * 2)
	{
		myAngle -= M_PI * 2;
	}

	thisObject->transform.x = (DIST * cos(myAngle)) + offsetX;
	thisObject->transform.y = (DIST * sin(myAngle)) + offsetY;
}