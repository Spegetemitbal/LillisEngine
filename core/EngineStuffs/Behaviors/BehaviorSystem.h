#pragma once

#include "PlayerController.h"
#include "Rotator.h"
//#include "Behavior.h"

DLLUSAGE class BehaviorSystem
{
public:

    static BehaviorSystem* getInstance();
    static BehaviorSystem* createInstance();
    static void delInstance();

    void InjectBehavior(std::string id, Behavior* behavior);
    //Debug purposes.
    void PrintBehaviors();
private:
    friend class BehaviorHandler;
    static BehaviorSystem* behaviorSystem;
    BehaviorSystem();
    ~BehaviorSystem();
    unordered_map<std::string,Behavior*> behaviorMap;
};