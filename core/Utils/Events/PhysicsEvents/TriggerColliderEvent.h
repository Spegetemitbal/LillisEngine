//
// Created by Somed on 5/24/2025.
//

#ifndef TRIGGERCOLLIDEREVENT_H
#define TRIGGERCOLLIDEREVENT_H

#include "../GameEvent.h"
#include "EngineStuffs/MemoryManagement/LilObj.h"
#include "EngineStuffs/Physics/RigidBody.h"

enum TriggerColliderEventType
{
    TCOL_ENTER = GameEventType::TRIGGER_ENTER_EVENT,
    TCOL_EXIT = GameEventType::TRIGGER_EXIT_EVENT,
    TCOL_STAY = GameEventType::TRIGGER_STAY_EVENT
};

class TriggerColliderEvent : public GameEvent
{
public:
    TriggerColliderEvent(LilObj<RigidBody> thisID, LilObj<RigidBody> otherID, int thisTag, int otherTag, TriggerColliderEventType type);
    ~TriggerColliderEvent() {};
    LilObj<RigidBody> getThisTrigger() const { return mFirst; };
    LilObj<RigidBody> getThatTrigger() const { return mSecond; };
    int getThisTag() const { return mFirstColTag; };
    int getThatTag() const { return mSecondColTag; };
    bool containsID(unsigned int ID) const override
    {
        return ID == mFirst.GetID() || ID == mSecond.GetID();
    };
private:
    LilObj<RigidBody> mFirst{};
    LilObj<RigidBody> mSecond{};
    int mFirstColTag = 0;
    int mSecondColTag = 0;
};



#endif //TRIGGERCOLLIDEREVENT_H
