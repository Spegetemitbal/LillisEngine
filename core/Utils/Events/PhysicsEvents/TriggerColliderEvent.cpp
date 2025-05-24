//
// Created by Somed on 5/24/2025.
//

#include "TriggerColliderEvent.h"

TriggerColliderEvent::TriggerColliderEvent(int thisID, int otherID, int thisTag, int otherTag, TriggerColliderEventType type) : GameEvent((GameEventType)type)
{
    mFirst = thisID;
    mSecond = otherID;
    mFirstColTag = thisTag;
    mSecondColTag = otherTag;
}