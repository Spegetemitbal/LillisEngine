#include "Rotator.h"

Behavior* Rotator::CreateRotator(char* loc)
{
	return new (loc)Rotator();
}

void Rotator::Deserialize(Serializer& ser)
{
	ser.GetFromBuffer<double>(&myAngle);
	ser.GetFromBuffer<float>(&offsetX);
	ser.GetFromBuffer<float>(&offsetY);
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