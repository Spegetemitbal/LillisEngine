//
// Created by Somed on 1/15/2025.
//

#ifndef CONTROLLERAXISEVENT_H
#define CONTROLLERAXISEVENT_H
#include "GameEvent.h"
#include "GLFW/glfw3.h"

namespace LILLIS
{
    enum ControllerAxes
    {
        AXIS_LS_X = GLFW_GAMEPAD_AXIS_LEFT_X,
        AXIS_LS_Y,
        AXIS_RS_X,
        AXIS_RS_Y,
        AXIS_LT,
        AXIS_RT
    };
}

class ControllerAxisEvent : public GameEvent
{
public:
    ControllerAxisEvent(int& axis, float& value, int id);
    ~ControllerAxisEvent() {};
    int GetAxis() const {return mAxis;}
    float GetValue() const {return mValue;}
    int GetControllerID() const {return controllerID;}
    bool containsID(unsigned int ID) const override {return ID == controllerID;}
private:
    int mAxis;
    float mValue;
    int controllerID = 0;
};



#endif //CONTROLLERAXISEVENT_H
