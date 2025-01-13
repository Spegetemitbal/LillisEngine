//
// Created by Somed on 1/13/2025.
//

#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include "GameEvent.h"

namespace LILLIS
{
    enum MouseButtonCode
    {
        LEFT_BUTTON = 0,
        RIGHT_BUTTON,
        MIDDLE_BUTTON
    };
}

class MouseEvent : public GameEvent
{
public:
    MouseEvent(const int& inputCode, const bool& down);
    int getInput() const { return mInputCode; }
    bool getPressed() const { return mDown; }
private:
    int mInputCode;
    bool mDown;
};



#endif //MOUSEEVENT_H
