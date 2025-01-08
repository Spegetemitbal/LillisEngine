//
// Created by Somed on 1/4/2025.
//

#include "StateChangeEvent.h"

StateChangeEvent::StateChangeEvent(int compID, int objID, const std::string& frmState, const std::string& newState) : GameEvent(STATE_CHANGE_EVENT)
{
    stateComponentID = compID;
    stateObjectID = objID;
    fromState = frmState;
    toState = newState;
}
