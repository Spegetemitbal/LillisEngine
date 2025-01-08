//
// Created by Somed on 1/4/2025.
//

#ifndef STATEOBJECT_H
#define STATEOBJECT_H

#include "Graphics/Animation.h"

class LilState
{

};

class StateObject {
public:
    std::string GetCurrentState(double& time);
    bool SetCurrentState(const std::string& state);
    void ForceCurrentState(const std::string& state);
private:

};



#endif //STATEOBJECT_H
