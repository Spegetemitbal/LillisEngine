//
// Created by Somed on 5/22/2025.
//

#include "PhysicsEventHandler.h"
#include "Utils/Events/PhysicsEvents/TriggerColliderEvent.h"
#include "Utils/Events/PhysicsEvents/CollisionEnterEvent.h"
#include "Utils/Events/PhysicsEvents/CollisionStayEvent.h"
#include "Utils/Events/PhysicsEvents/CollisionExitEvent.h"

void PhysicsEventHandler::TickFireEvent()
{
    EventSystem* ev = EventSystem::getInstance();

    for (auto it : collisionMatrix)
    {
        bool isTrigger = false;
        RigidBody* rbA = it.first.first;
        RigidBody* rbB = it.first.second;
        if (it.second.BodyA->isTrigger || it.second.BodyB->isTrigger)
        {
            isTrigger = true;
        }

        if (persistingCollisions.contains(it.first))
        {
            //Collision Stay
            if (isTrigger)
            {
                ev->fireEvent(TriggerColliderEvent(rbA->GetID(), rbB->GetID(), rbA->GetColTag(), rbB->GetColTag(),
                    TriggerColliderEventType::TCOL_STAY));
            } else
            {
                ev->fireEvent(CollisionStayEvent(rbA->GetID(), rbB->GetID(), rbA->GetColTag(), rbB->GetColTag(),
                    it.second.Normal,it.second.Depth,it.second.ContactCount,it.second.Contact1,it.second.Contact2));
            }
        } else
        {
            //Collision Enter
            if (isTrigger)
            {
                ev->fireEvent(TriggerColliderEvent(rbA->GetID(), rbB->GetID(), rbA->GetColTag(), rbB->GetColTag(),
                    TriggerColliderEventType::TCOL_ENTER));
            } else
            {
                ev->fireEvent(CollisionEnterEvent(rbA->GetID(), rbB->GetID(), rbA->GetColTag(), rbB->GetColTag(),
                it.second.Normal,it.second.Depth,it.second.ContactCount,it.second.Contact1,it.second.Contact2));
            }
        }
    }

    //Collision Exit
    for (auto it : leftCollision)
    {
        RigidBody* rbA = it.first;
        RigidBody* rbB = it.second;
        if (rbA->isTrigger || rbB->isTrigger)
        {
            ev->fireEvent(TriggerColliderEvent(rbA->GetID(), rbB->GetID(), rbA->GetColTag(), rbB->GetColTag(),
                TriggerColliderEventType::TCOL_EXIT));
            continue;
        }
        //TODO: Triggers need love too...
        ev->fireEvent(CollisionExitEvent(rbA->GetID(), rbB->GetID(), rbA->GetColTag(), rbB->GetColTag()));
    }
    leftCollision.clear();
}
