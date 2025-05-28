//
// Created by Somed on 5/24/2025.
//

#include "TriggerColliderEvent.h"

TriggerColliderEvent::TriggerColliderEvent(LilObj<RigidBody> thisID, LilObj<RigidBody> otherID, int thisTag, int otherTag, TriggerColliderEventType type) : GameEvent((GameEventType)type)
{
    mFirst = thisID;
    mSecond = otherID;
    mFirstColTag = thisTag;
    mSecondColTag = otherTag;
}