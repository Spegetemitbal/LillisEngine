#pragma once
#include "Color.h"
#include "../Component.h"

class GameObject;

class RectangleRenderer : public Component
{
public:

	static Component* CreateRectangleRenderer(GameObject* G, int* param);

	RectangleRenderer() { height = 0; width = 0; color = Color(0, 0, 0); }
	RectangleRenderer(Color c, float w, float h) { color = c; width = w; height = h; }

	Color getColor() { return color; }
	float getWidth() { return width; }
	float getHeight() { return height; }

	void setColor(Color c) { color = c; }
	void setWidth(float w) { width = w; }
	void setHeight(float h) { height = h; }

private:

	Color color;
	float width;
	float height;
};