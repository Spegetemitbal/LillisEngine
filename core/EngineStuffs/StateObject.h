//
// Created by Somed on 1/4/2025.
//

#ifndef STATEOBJECT_H
#define STATEOBJECT_H

#include <unordered_set>

#include "Graphics/Animation.h"

enum LilStateChangeConditions
{
    COND_USER_DEF = 0,
    COND_USER_END_ANIM,
    COND_END_ANIM,
    COND_END_ANIM_RETURN,
    COND_FIXED_TIME,
    COND_FIXED_TIME_RETURN
};

struct LilState
{
    std::string name;
    Animation* anim;
    double maxStateTime = 0;
    std::unordered_map<std::string, LilStateChangeConditions> toStates;
};

class StateObject {
public:

    explicit StateObject(const std::string& n) {systemName = n;};
    StateObject() = delete;
    ~StateObject() = default;

    void AddNewState(const LilState& state)
    {
        states.emplace(state.name, state);
    }

    std::string GetName() const { return systemName; }
    void SetDefaultState(const std::string& newDefault) { defaultState = newDefault; }
    std::string GetDefaultState() const { return defaultState; }

    //std::string GetNextState(const std::string& currentState, double time = 0);
    LilState* GetStateByName(const std::string& stateName) { return &states[stateName]; }
    bool GetValidStateChange(const std::string& currentState, const std::string& newState);
private:
    std::string systemName;
    std::string defaultState;
    std::unordered_map<std::string, LilState> states;
};



#endif //STATEOBJECT_H
