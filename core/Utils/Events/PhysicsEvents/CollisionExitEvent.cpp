#include "CollisionExitEvent.h"

CollisionExitEvent::CollisionExitEvent(LilObj<RigidBody> thisID, LilObj<RigidBody> otherID, int thisTag, int otherTag) : GameEvent(COLLISION_EXIT_EVENT)
{
	mFirst = thisID;
	mSecond = otherID;
	mFirstColTag = thisTag;
	mSecondColTag = otherTag;
}