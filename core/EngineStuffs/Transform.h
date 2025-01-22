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

	glm::vec3 LocalPosition() const {return localPosition;}
	void Translate(glm::vec3 translation) {localPosition += translation; hasUpdated = true;}
	void SetLocalPosition(glm::vec3 pos) {localPosition = pos; hasUpdated = true;}

	float LocalRotation() const {return localRotation;}
	void Rotate(float rotation) {localRotation += rotation;}
	void SetLocalRotation(float rotation) {localRotation = rotation;}

	glm::vec2 LocalScale() const {return localScale;}
	void SetLocalScale(glm::vec2 scale) {localScale = scale;}

	glm::vec3 GlobalPosition() const {return globalPosition;};
	float GlobalRotation() const {return globalRotation;};
	glm::vec2 GlobalScale() const {return globalScale;};

private:

	bool hasUpdated = false;

	glm::vec3 localPosition;
	float localRotation = 0;
	glm::vec2 localScale = { 1.0f, 1.0f };

	friend class SceneGraph;
	glm::vec3 globalPosition;
	float globalRotation;
	glm::vec2 globalScale;
};
