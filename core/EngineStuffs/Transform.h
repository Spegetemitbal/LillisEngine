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
	float localRotation = 0;
	glm::vec2 localScale = { 1.0f, 1.0f };

	glm::vec3 GlobalPosition() const {return globalPosition;};
	float GlobalRotation() const {return globalRotation;};
	glm::vec2 GlobalScale() const {return globalScale;};

private:
	friend class SceneGraph;
	glm::vec3 globalPosition;
	float globalRotation;
	glm::vec2 globalScale;
};
