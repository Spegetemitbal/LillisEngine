#pragma once
#include "../GameEvent.h"
#include "EngineStuffs/MemoryManagement/LilObj.h"
#include "EngineStuffs/Physics/RigidBody.h"

class CollisionExitEvent : public GameEvent
{
public:
	CollisionExitEvent(LilObj<RigidBody> thisID, LilObj<RigidBody> otherID, int thisTag, int otherTag);
	~CollisionExitEvent() {};
	LilObj<RigidBody> getThisCollider() const { return mFirst; };
	LilObj<RigidBody> getThatCollider() const { return mSecond; };
	int getThisTag() const { return mFirstColTag; };
	int getThatTag() const { return mSecondColTag; };
	bool containsID(unsigned int ID) const override
	{
		if (mSecond.Exists())
		{
			return ID == mFirst.GetID() || ID == mSecond.GetID();
		}
		return ID == mFirst.GetID();
	};
private:
	LilObj<RigidBody> mFirst{};
	LilObj<RigidBody> mSecond{};
	int mFirstColTag = 0;
	int mSecondColTag = 0;
};

