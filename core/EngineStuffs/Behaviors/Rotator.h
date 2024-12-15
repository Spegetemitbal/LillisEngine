#pragma once
#include "Behavior.h"

class GameObject;

class Rotator : public Behavior
{
public:

	static Component* CreateRotator(GameObject* G, int* param);
	Rotator() : Behavior("Rotator") { myAngle = 0; }
	//Angle in Radians
	Rotator(double angle) : Behavior("Rotator") { myAngle = angle; };
	void Update(float deltaTime);

	void setAngle(double angle) { myAngle = angle; };
	void setBaseOffset(float x, float y) { offsetX = x; offsetY = y; }

private:
	double myAngle;
	float offsetX = 0, offsetY = 0;
};