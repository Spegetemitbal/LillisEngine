#pragma once
#include "Component.h"

class RectangleCollider : public Component
{
public:
	RectangleCollider() { colliderHeight = 0; colliderWidth = 0; }
	RectangleCollider(float w, float h) { colliderHeight = h; colliderWidth = w; }

	static Component* CreateRectangleCollider(GameObject* G, int* param);

	bool CheckCollision(RectangleCollider other);

	float getHeight() { return colliderHeight; };
	float getWidth() { return colliderWidth; };

	bool getColliding() { return isCurrentlyColliding; }

	bool isCurrentlyColliding = false;

private:

	float colliderWidth;
	float colliderHeight;
};