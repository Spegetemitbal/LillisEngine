//
// Created by Somed on 1/4/2025.
//

#ifndef STATECHANGEEVENT_H
#define STATECHANGEEVENT_H

#include "GameEvent.h"

class StateChangeEvent : public GameEvent {
public:
    StateChangeEvent(unsigned int compID, const std::string& objID, const std::string& frmState, const std::string& newState);
    ~StateChangeEvent() {};
    int getCompID() const {return stateComponentID;}
    std::string getObjID() const {return stateObjectID;}
    std::string getFromState() const {return fromState;}
    std::string getToState() const {return toState;}
    bool containsID(unsigned int ID) const override
    {
        return ID == stateComponentID;
    };
private:
    std::string fromState, toState, stateObjectID;
    unsigned int stateComponentID;
};



#endif //STATECHANGEEVENT_H
