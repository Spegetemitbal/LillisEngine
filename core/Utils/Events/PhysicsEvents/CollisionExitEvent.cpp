#include "CollisionExitEvent.h"

CollisionExitEvent::CollisionExitEvent(int thisID, int otherID, int thisTag, int otherTag) : GameEvent(COLLISION_EXIT_EVENT)
{
	mFirst = thisID;
	mSecond = otherID;
	mFirstColTag = thisTag;
	mSecondColTag = otherTag;
}