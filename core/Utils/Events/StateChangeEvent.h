//
// Created by Somed on 1/4/2025.
//

#ifndef STATECHANGEEVENT_H
#define STATECHANGEEVENT_H

#include "GameEvent.h"
#include "EngineStuffs/Graphics/Animator.h"

class StateChangeEvent : public GameEvent {
public:
    StateChangeEvent(LilObj<Animator> compID, const std::string& objID, const std::string& frmState, const std::string& newState);
    ~StateChangeEvent() {};
    LilObj<Animator> getCompID() const {return stateComponentID;}
    std::string getObjID() const {return stateObjectID;}
    std::string getFromState() const {return fromState;}
    std::string getToState() const {return toState;}
    bool containsID(unsigned int ID) const override
    {
        return ID == stateComponentID.GetID();
    };
private:
    std::string fromState, toState, stateObjectID;
    LilObj<Animator> stateComponentID{};
};



#endif //STATECHANGEEVENT_H
