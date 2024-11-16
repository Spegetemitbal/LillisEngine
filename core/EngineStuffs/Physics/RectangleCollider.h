#pragma once
#include "../Component.h"

class RectangleCollider : public Component
{
public:
	RectangleCollider() { colliderHeight = 0; colliderWidth = 0; }
	RectangleCollider(float w, float h, int id) { colliderHeight = h; colliderWidth = w; tag = id; }

	static Component* CreateRectangleCollider(GameObject* G, int* param);

	bool CheckCollision(RectangleCollider other);

	void setWidth(float w) { colliderWidth = w; };
	void setHeight(float h) { colliderHeight = h; };
	void setTag(int t) { tag = t; };
	float getHeight() { return colliderHeight; };
	float getWidth() { return colliderWidth; };
	int getTag() { return tag; };

	int collidingWith = -1;

private:

	int tag = -1;
	float colliderWidth;
	float colliderHeight;
};