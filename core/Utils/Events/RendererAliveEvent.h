//
// Created by Somed on 12/23/2024.
//

#ifndef RENDERERALIVEEVENT_H
#define RENDERERALIVEEVENT_H

#include "GameEvent.h"

class RendererAliveEvent : public GameEvent
{
public:
    RendererAliveEvent(unsigned int ID, bool enabled) : GameEvent(RENDERER_ALIVE_EVENT)
    {
        mID = ID;
        mEnabled = enabled;
    }
    ~RendererAliveEvent() = default;
    unsigned int getID() const {return mID;};
    unsigned int getEnabled() const {return mEnabled;};
private:
    unsigned int mID;
    bool mEnabled;
};

#endif //RENDERERALIVEEVENT_H
