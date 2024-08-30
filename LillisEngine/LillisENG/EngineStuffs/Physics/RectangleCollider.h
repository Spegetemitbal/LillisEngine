#pragma once
#include "../Component.h"

class RectangleCollider : public Component
{
public:
	RectangleCollider() { colliderHeight = 0; colliderWidth = 0; }
	RectangleCollider(float w, float h, int id) { colliderHeight = h; colliderWidth = w; tag = id; }

	static Component* CreateRectangleCollider(GameObject* G, int* param);

	//void initComponent();

	bool CheckCollision(RectangleCollider other);

	float getHeight() { return colliderHeight; };
	float getWidth() { return colliderWidth; };

	int collidingWith = -1;

private:

	int tag = -1;
	float colliderWidth;
	float colliderHeight;
};