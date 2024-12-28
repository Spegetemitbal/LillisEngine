//
// Created by Somed on 12/23/2024.
//

#ifndef COLLIDERALIVEEVENT_H
#define COLLIDERALIVEEVENT_H

#include "GameEvent.h"

class ColliderAliveEvent : public GameEvent
{
public:
    ColliderAliveEvent(unsigned int ID, bool enabled) : GameEvent(COLLIDER_ALIVE_EVENT)
    {
        mID = ID;
        mEnabled = enabled;
    }
    ~ColliderAliveEvent() = default;
    unsigned int getID() const {return mID;};
    unsigned int getEnabled() const {return mEnabled;};
private:
    unsigned int mID;
    bool mEnabled;
};

#endif //COLLIDERALIVEEVENT_H
