#pragma once
#include <glm/vec3.hpp>

class Transform
{
public:
	Transform(float xSet = 0, float ySet = 0, float zSet = 0)
	{
		localPosition.x = xSet;
		localPosition.y = ySet;
		localPosition.z = zSet;
	}
	glm::vec3 localPosition;

	glm::vec3 GlobalPosition() const {return globalPosition;};

private:
	friend class SceneGraph;
	glm::vec3 globalPosition;
};
