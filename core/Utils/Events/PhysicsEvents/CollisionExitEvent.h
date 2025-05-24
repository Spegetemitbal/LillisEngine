#pragma once
#include "../GameEvent.h"

class CollisionExitEvent : public GameEvent
{
public:
	CollisionExitEvent(int thisID, int otherID, int thisTag, int otherTag);
	~CollisionExitEvent() {};
	int getThisCollider() const { return mFirst; };
	int getThatCollider() const { return mSecond; };
	int getThisTag() const { return mFirstColTag; };
	int getThatTag() const { return mSecondColTag; };
private:
	int mFirst = -1;
	int mSecond = -1;
	int mFirstColTag = 0;
	int mSecondColTag = 0;
};

