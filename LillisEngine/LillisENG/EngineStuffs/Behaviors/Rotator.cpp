#include "Rotator.h"
#include "../GameObject.h"
#define _USE_MATH_DEFINES
#include <math.h>

Component* Rotator::CreateRotator(GameObject* G, int* params)
{
	return G->CreateRotator(params[0]);
}

void Rotator::Update(float deltaTime)
{
	const int OFFSET = 300;
	const int DIST = 100;
	myAngle += deltaTime / 2;

	if (myAngle > M_PI * 2)
	{
		myAngle -= M_PI * 2;
	}

	thisObject->transform.x = (DIST * cos(myAngle)) + OFFSET;
	thisObject->transform.y = (DIST * sin(myAngle)) + OFFSET;
}