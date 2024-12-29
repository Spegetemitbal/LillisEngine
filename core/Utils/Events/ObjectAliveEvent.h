//
// Created by Somed on 12/23/2024.
//

#ifndef OBJECTALIVEEVENT_H
#define OBJECTALIVEEVENT_H

#include "GameEvent.h"

class ObjectAliveEvent : public GameEvent
{
public:
    ObjectAliveEvent(unsigned int ID, bool enabled, bool deleted);
    ~ObjectAliveEvent() = default;
    unsigned int getID() const {return mID;};
    bool getEnabled() const {return mEnabled;};
    bool getDeleted() const {return mDeleted;};
private:
    unsigned int mID = 0;
    bool mEnabled = false;
    bool mDeleted = false;
};

#endif //OBJECTALIVEEVENT_H
