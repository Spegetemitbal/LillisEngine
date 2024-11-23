#include "CollisionEvent.h"

CollisionEvent::CollisionEvent(int thisID, int otherID) : GameEvent(COLLISION_EVENT)
{
	mFirst = thisID;
	mSecond = otherID;
}