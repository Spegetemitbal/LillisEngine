//
// Created by Somed on 5/22/2025.
//

#include "PhysicsEventHandler.h"
#include "Utils/Events/PhysicsEvents/TriggerColliderEvent.h"
#include "Utils/Events/PhysicsEvents/CollisionEnterEvent.h"
#include "Utils/Events/PhysicsEvents/CollisionStayEvent.h"
#include "Utils/Events/PhysicsEvents/CollisionExitEvent.h"

void PhysicsEventHandler::TickFireEvent(MemoryPool* memPool)
{
    EventSystem* ev = EventSystem::getInstance();

    for (auto it : collidedThisFrame)
    {
        bool isTrigger = false;
        RigidBody* rbA = it.first.first;
        RigidBody* rbB = it.first.second;
        if (rbA->isTrigger || rbB->isTrigger)
        {
            isTrigger = true;
        }

        if (!collidedLastFrame.contains(it.first))
        {
            //Tile stuffs.
            if (rbA == rbB)
            {
                //Collision Enter
                if (isTrigger)
                {
                    ev->fireEvent(TriggerColliderEvent({memPool, rbA->GetID()}, {}, rbA->GetColTag(), tileCollisions[rbA],
                        TriggerColliderEventType::TCOL_ENTER));
                } else
                {
                    ev->fireEvent(CollisionEnterEvent({memPool, rbA->GetID()}, {}, rbA->GetColTag(), tileCollisions[rbA],
                    it.second.Normal,it.second.Depth,it.second.ContactCount,it.second.Contact1,it.second.Contact2));
                }
                colliderCache.insert(it.first);
                continue;
            }

            //Collision Enter
            if (isTrigger)
            {
                ev->fireEvent(TriggerColliderEvent({memPool, rbA->GetID()}, {memPool, rbB->GetID()}, rbA->GetColTag(), rbB->GetColTag(),
                    TriggerColliderEventType::TCOL_ENTER));
            } else
            {
                ev->fireEvent(CollisionEnterEvent({memPool, rbA->GetID()}, {memPool, rbB->GetID()}, rbA->GetColTag(), rbB->GetColTag(),
                it.second.Normal,it.second.Depth,it.second.ContactCount,it.second.Contact1,it.second.Contact2));
            }
            colliderCache.insert(it.first);
        }
    }

    //Testing existing colliders.
    for (auto it : collidedLastFrame)
    {
        bool isTrigger = false;
        RigidBody* rbA = it.first;
        RigidBody* rbB = it.second;
        if (rbA->isTrigger || rbB->isTrigger)
        {
            isTrigger = true;
        }

        if (collidedThisFrame.contains(it))
        {
            ColManifold& col = collidedThisFrame[it];
            if (!colliderCache.contains(it))
            {
                if (rbA == rbB)
                {
                    //Collision Stay
                    if (isTrigger)
                    {
                        ev->fireEvent(TriggerColliderEvent({memPool, rbA->GetID()}, {}, rbA->GetColTag(), tileCollisions[rbA],
                            TriggerColliderEventType::TCOL_STAY));
                    } else
                    {
                        ev->fireEvent(CollisionStayEvent({memPool,rbA->GetID()}, {}, rbA->GetColTag(), tileCollisions[rbA],
                            col.Normal, col.Depth,col.ContactCount,col.Contact1,col.Contact2));
                    }
                    continue;
                }

                //Collision Stay
                if (isTrigger)
                {
                    ev->fireEvent(TriggerColliderEvent({memPool, rbA->GetID()}, {memPool, rbB->GetID()}, rbA->GetColTag(), rbB->GetColTag(),
                        TriggerColliderEventType::TCOL_STAY));
                } else
                {
                    ev->fireEvent(CollisionStayEvent({memPool,rbA->GetID()}, {memPool,rbB->GetID()}, rbA->GetColTag(), rbB->GetColTag(),
                        col.Normal, col.Depth,col.ContactCount,col.Contact1,col.Contact2));
                }
            }
        } else
        {
            toRemove.push_back(it);

            //Making a collider Inactive should not trigger a collision exit.
            if (!rbA->GetActive() || !rbB->GetActive())
            {
                continue;
            }

            if (rbA == rbB)
            {
                //Collision Exit
                if (rbA->isTrigger || rbB->isTrigger)
                {
                    ev->fireEvent(TriggerColliderEvent({memPool, rbA->GetID()}, {}, rbA->GetColTag(), tileCollisions[rbA],
                        TriggerColliderEventType::TCOL_EXIT));
                    continue;
                }
                ev->fireEvent(CollisionExitEvent({memPool, rbA->GetID()}, {}, rbA->GetColTag(), tileCollisions[rbA]));
                continue;
            }

            //Collision Exit
            if (rbA->isTrigger || rbB->isTrigger)
            {
                ev->fireEvent(TriggerColliderEvent({memPool, rbA->GetID()}, {memPool, rbB->GetID()}, rbA->GetColTag(), rbB->GetColTag(),
                    TriggerColliderEventType::TCOL_EXIT));
                continue;
            }
            ev->fireEvent(CollisionExitEvent({memPool, rbA->GetID()}, {memPool, rbB->GetID()}, rbA->GetColTag(), rbB->GetColTag()));
        }
    }

    //Remove null collisions from persistent.
    for (auto it : toRemove)
    {
        collidedLastFrame.erase(it);
        if (it.first == it.second)
        {
            tileCollisions.erase(it.first);
        }
    }

    //Add new collisions to persistent.
    for (auto it : colliderCache)
    {
        collidedLastFrame.emplace(it);
    }

    collidedThisFrame.clear();
    colliderCache.clear();
    toRemove.clear();
}
