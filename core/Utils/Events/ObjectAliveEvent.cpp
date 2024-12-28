//
// Created by Somed on 12/28/2024.
//

#include "ObjectAliveEvent.h"

ObjectAliveEvent::ObjectAliveEvent(unsigned int ID, bool enabled, bool deleted) : GameEvent(OBJECT_ALIVE_EVENT)
{
    mID = ID;
    mEnabled = enabled;
    mDeleted = deleted;
}