//
// Created by Somed on 1/15/2025.
//

#include "ControllerAxisEvent.h"

ControllerAxisEvent::ControllerAxisEvent(int &axis, float &value, int id) : GameEvent(CONTROLLER_AXIS_EVENT)
{
    mAxis = axis;
    mValue = value;
    this->controllerID = id;
}
