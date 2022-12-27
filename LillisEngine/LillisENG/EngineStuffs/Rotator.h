#pragma once
#include "Component.h"

class GameObject;

class Rotator : public Component
{
public:

	static Component* CreateRotator(GameObject* G, int* param);
	Rotator() { myAngle = 0; }
	//Angle in Radians
	Rotator(double angle) { myAngle = angle; };
	void Update(float deltaTime);

private:
	double myAngle;
};