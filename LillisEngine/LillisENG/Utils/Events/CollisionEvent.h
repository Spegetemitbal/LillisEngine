#pragma once
#include "GameEvent.h"
class CollisionEvent : public GameEvent
{
public:
	CollisionEvent(int thisID, int otherID);
	~CollisionEvent() {};
	int getThisCollider() const { return mFirst; };
	int getThatCollider() const { return mSecond; };
private:
	int mFirst = -1;
	int mSecond = -1;
};

