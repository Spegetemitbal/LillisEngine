//
// Created by Somed on 1/4/2025.
//

#include "StateChangeEvent.h"

StateChangeEvent::StateChangeEvent(unsigned int compID, const std::string& objID, const std::string& frmState, const std::string& newState) : GameEvent(STATE_CHANGE_EVENT)
{
    stateComponentID = compID;
    stateObjectID = objID;
    fromState = frmState;
    toState = newState;
}
