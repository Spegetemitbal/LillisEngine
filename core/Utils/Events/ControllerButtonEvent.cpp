//
// Created by Somed on 1/15/2025.
//

#include "ControllerButtonEvent.h"

ControllerButtonEvent::ControllerButtonEvent(const int &inputCode, const bool &down, int id) : GameEvent(CONTROLLER_BUTTON_EVENT)
{
    mInputCode = inputCode;
    mDown = down;
    controllerID = id;
}
