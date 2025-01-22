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
	thisObject->transform.SetLocalPosition(
		{(DIST * cos(myAngle)) + offsetX, (DIST * sin(myAngle)) + offsetY, thisObject->transform.LocalPosition().z});
}