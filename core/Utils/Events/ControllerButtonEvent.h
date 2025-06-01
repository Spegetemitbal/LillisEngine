//
// Created by Somed on 1/15/2025.
//

#ifndef CONTROLLERBUTTONEVENT_H
#define CONTROLLERBUTTONEVENT_H
#include "GameEvent.h"
#include "GLFW/glfw3.h"

namespace LILLIS
{
    enum ControllerButtons
    {
        BUTTON_SOUTH = GLFW_GAMEPAD_BUTTON_A,
        BUTTON_EAST,
        BUTTON_WEST,
        BUTTON_NORTH,
        BUTTON_LB,
        BUTTON_RB,
        BUTTON_BACK,
        BUTTON_START,
        BUTTON_GUIDE,
        BUTTON_LS,
        BUTTON_RS,
        BUTTON_DPAD_UP,
        BUTTON_DPAD_RIGHT,
        BUTTON_DPAD_DOWN,
        BUTTON_DPAD_LEFT
    };
}

class ControllerButtonEvent : public GameEvent {
public:
    ControllerButtonEvent(const int& inputCode, const bool& down, int id);
    ~ControllerButtonEvent() {};
    int getInput() const { return mInputCode; };
    bool getPressed() const { return mDown; };
    int GetControllerID() const {return controllerID;}
    bool containsID(unsigned int ID) const override {return ID == controllerID;}
private:
    int mInputCode;
    bool mDown;
    int controllerID = 0;
};



#endif //CONTROLLERBUTTONEVENT_H
