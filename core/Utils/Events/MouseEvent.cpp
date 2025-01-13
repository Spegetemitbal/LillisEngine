//
// Created by Somed on 1/13/2025.
//

#include "MouseEvent.h"

MouseEvent::MouseEvent(const int &inputCode, const bool &down) : GameEvent(MOUSE_INPUT_EVENT)
{
    mInputCode = inputCode;
    mDown = down;
}
