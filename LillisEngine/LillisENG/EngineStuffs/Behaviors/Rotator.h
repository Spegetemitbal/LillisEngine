#pragma once
#include "../Component.h"

class GameObject;

class Rotator : public Component
{
public:

	static Component* CreateRotator(GameObject* G, int* param);
	Rotator() { myAngle = 0;}
	//Angle in Radians
	Rotator(double angle) { myAngle = angle; };
	void Update(float deltaTime);

	void setBaseOffset(float x, float y) { offsetX = x; offsetY = y; }

private:
	double myAngle;
	float offsetX = 0, offsetY = 0;
};