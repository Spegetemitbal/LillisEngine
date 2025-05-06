#pragma once
#include <glm/trigonometric.hpp>
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
	void Translate(glm::vec2 translation)
	{
		if (translation == glm::vec2(0))
		{
			return;
		}
		localPosition += translation;
		toUpdate = true;
	}
	void SetLocalPosition(glm::vec2 pos) {localPosition = pos; toUpdate = true;}

	float LocalRotation() const {return localRotation;}
	void Rotate(float rotation, bool degrees = false)
	{
		if (rotation == 0.0f)
		{
			return;
		}
		if (degrees)
		{
			localRotation += glm::radians(rotation);
		}
		else
		{
			localRotation += rotation;
		}
		toUpdate = true;
	}
	void SetLocalRotation(float rotation, bool degrees = false)
	{
		if (degrees)
		{
			localRotation = glm::radians(rotation);
		}
		else
		{
			localRotation = rotation;
		}
		toUpdate = true;
	}

	glm::vec2 LocalScale() const {return localScale;}
	void SetLocalScale(glm::vec2 scale) {localScale = scale;}

	glm::vec2 GlobalPosition() const
	{
		if (isChild)
		{
			return globalPosition;
		}
		return localPosition;
	};
	float GlobalRotation() const
	{
		if (isChild)
		{
			return globalRotation;
		}
		return localRotation;
	};
	glm::vec2 GlobalScale() const
	{
		if (isChild)
		{
			return globalScale;
		}
		return localScale;
	};

	bool getIsChild() const {return isChild;};
	bool getToUpdate() const {return toUpdate;};

private:

	bool isChild = false;
	bool toUpdate = true;

	glm::vec2 localPosition;
	//In radians.
	float localRotation = 0;
	glm::vec2 localScale = { 1.0f, 1.0f };

	friend class SceneGraph;
	glm::vec2 globalPosition;
	float globalRotation;
	glm::vec2 globalScale;
};
