//
// Created by Somed on 5/24/2025.
//

#ifndef TRIGGERCOLLIDEREVENT_H
#define TRIGGERCOLLIDEREVENT_H

#include "../GameEvent.h"

enum TriggerColliderEventType
{
    TCOL_ENTER = GameEventType::TRIGGER_ENTER_EVENT,
    TCOL_EXIT = GameEventType::TRIGGER_EXIT_EVENT,
    TCOL_STAY = GameEventType::TRIGGER_STAY_EVENT
};

class TriggerColliderEvent : public GameEvent
{
public:
    TriggerColliderEvent(int thisID, int otherID, int thisTag, int otherTag, TriggerColliderEventType type);
    ~TriggerColliderEvent() {};
    int getThisTrigger() const { return mFirst; };
    int getThatTrigger() const { return mSecond; };
    int getThisTag() const { return mFirstColTag; };
    int getThatTag() const { return mSecondColTag; };
    bool containsID(unsigned int ID) const override
    {
        return ID == mFirst || ID == mSecond;
    };
private:
    int mFirst = -1;
    int mSecond = -1;
    int mFirstColTag = 0;
    int mSecondColTag = 0;
};



#endif //TRIGGERCOLLIDEREVENT_H
