//
// Created by Somed on 1/4/2025.
//

#include "StateObject.h"



bool StateObject::GetValidStateChange(const std::string& currentState, const std::string& newState)
{
    try
    {
        LilState currState = states[currentState];
        if (currState.toStates.find(newState) != currState.toStates.end())
        {
            if (currState.toStates[newState] == COND_USER_DEF)
            {
                return true;
            }
            return false;
        }
        return false;

    } catch (...)
    {
        std::cout << "Nonexistent state detected" << '\n';
        return false;
    }
}
