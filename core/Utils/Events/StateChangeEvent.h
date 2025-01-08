//
// Created by Somed on 1/4/2025.
//

#ifndef STATECHANGEEVENT_H
#define STATECHANGEEVENT_H

#include "GameEvent.h"

class StateChangeEvent : public GameEvent {
public:
    StateChangeEvent(int compID, int objID, const std::string& frmState, const std::string& newState);
    ~StateChangeEvent() {};
    int getCompID() const {return stateComponentID;}
    int getObjID() const {return stateObjectID;}
    std::string getFromState() const {return fromState;}
    std::string getToState() const {return toState;}
private:
    std::string fromState, toState;
    int stateComponentID, stateObjectID;
};



#endif //STATECHANGEEVENT_H
