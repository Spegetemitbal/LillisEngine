#pragma once

class Transform
{
public:
	Transform(float xSet = 0, float ySet = 0, float zSet = 0)
	{
		x = xSet;
		y = ySet;
		z = zSet;
	}

	float x;
	float y;
	float z;

};