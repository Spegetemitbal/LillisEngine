#pragma once

#include "RectangleCollider.h"

class PhysicsSystem : public EventListener
{
public:
	PhysicsSystem();
	~PhysicsSystem() {}

	//Do a similar thing to the graphicssystem and move all the collision checking here!

	bool checkReset() { return doReset; }
	void patchReset() { doReset = false; }

private:

	bool doReset = false;
	void handleEvent(const Event& theEvent);
};