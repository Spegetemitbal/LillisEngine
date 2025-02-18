#pragma once
#include <glm/vec2.hpp>
#include "Component.h"

class Transform : public Component
{
public:
	Transform(float xSet = 0, float ySet = 0)
	{
		localPosition = glm::vec2(xSet, ySet);
	}

	glm::vec2 LocalPosition() const {return localPosition;}
	void Translate(glm::vec2 translation) {localPosition += translation; toUpdate = true;}
	void SetLocalPosition(glm::vec2 pos) {localPosition = pos; toUpdate = true;}

	float LocalRotation() const {return localRotation;}
	void Rotate(float rotation) {localRotation += rotation;}
	void SetLocalRotation(float rotation) {localRotation = rotation;}

	glm::vec2 LocalScale() const {return localScale;}
	void SetLocalScale(glm::vec2 scale) {localScale = scale;}

	glm::vec2 GlobalPosition() const {return globalPosition;};
	float GlobalRotation() const {return globalRotation;};
	glm::vec2 GlobalScale() const {return globalScale;};

private:

	bool toUpdate = true;

	glm::vec2 localPosition;
	float localRotation = 0;
	glm::vec2 localScale = { 1.0f, 1.0f };

	friend class SceneGraph;
	glm::vec2 globalPosition;
	float globalRotation;
	glm::vec2 globalScale;
};
