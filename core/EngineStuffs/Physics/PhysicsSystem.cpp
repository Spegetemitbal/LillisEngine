#include "pch.h"
#include "PhysicsSystem.h"
#include "Utils/Events/CollisionEvent.h"

PhysicsSystem::PhysicsSystem()
{
	EventSystem* e = EventSystem::getInstance();
	e->addListener((EventType)COLLISION_EVENT, this);
}

void PhysicsSystem::handleEvent(const Event& theEvent)
{
	if (theEvent.getType() == COLLISION_EVENT)
	{
		const CollisionEvent& colEvent = static_cast<const CollisionEvent&>(theEvent);
		int id1 = colEvent.getThisCollider();
		int id2 = colEvent.getThatCollider();

		if (id1 == 0 && id2 == 2 || id1 == 1 && id2 == 3)
		{
			std::cout << "Player1 Wins!" << '\n';
			doReset = true;
		}

		if (id1 == 1 && id2 == 2 || id1 == 0 && id2 == 3)
		{
			std::cout << "Player2 Wins!" << '\n';
			doReset = true;
		}
	}
}