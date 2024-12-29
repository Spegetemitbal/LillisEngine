#pragma once
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

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
	unsigned int localRotation;
	glm::vec2 localScale;

	glm::vec3 GlobalPosition() const {return globalPosition;};
	unsigned int GlobalRotation() const {return globalRotation;};
	glm::vec2 GlobalScale() const {return globalScale;};

private:
	friend class SceneGraph;
	glm::vec3 globalPosition;
	unsigned int globalRotation;
	glm::vec2 globalScale;
};
