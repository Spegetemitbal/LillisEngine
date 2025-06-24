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
        LilObj<RigidBody> rbA = {memPool,it.first.first};
        LilObj<RigidBody> rbB = {memPool,it.first.second};
        if (rbA->isTrigger || rbB->isTrigger)
        {
            isTrigger = true;
        }

        std::pair<unsigned int, unsigned int> IDs = std::make_pair(rbA->GetID(), rbB->GetID());
        if (!collidedLastFrame.contains(IDs))
        {
            //Tile stuffs.
            if (it.first.first == it.first.second)
            {
                //Collision Enter
                if (isTrigger)
                {
                    ev->fireEvent(TriggerColliderEvent(rbA, {}, rbA->GetColTag(), tileCollisions[rbA.GetID()],
                        TriggerColliderEventType::TCOL_ENTER));
                } else
                {
                    ev->fireEvent(CollisionEnterEvent(rbA, {}, rbA->GetColTag(), tileCollisions[rbA.GetID()],
                    it.second.Normal,it.second.Depth,it.second.ContactCount,it.second.Contact1,it.second.Contact2));
                }
                colliderCache.insert(it.first);
                continue;
            }

            //Collision Enter
            if (isTrigger)
            {
                ev->fireEvent(TriggerColliderEvent(rbA, rbB, rbA->GetColTag(), rbB->GetColTag(),
                    TriggerColliderEventType::TCOL_ENTER));
            } else
            {
                ev->fireEvent(CollisionEnterEvent(rbA, rbB, rbA->GetColTag(), rbB->GetColTag(),
                it.second.Normal,it.second.Depth,it.second.ContactCount,it.second.Contact1,it.second.Contact2));
            }
            colliderCache.insert(it.first);
        }
    }

    //Testing existing colliders.
    for (auto it : collidedLastFrame)
    {
        bool isTrigger = false;
        LilObj<RigidBody> rbA = {memPool, it.first};
        LilObj<RigidBody> rbB = {memPool,it.second};
        if (rbA->isTrigger || rbB->isTrigger)
        {
            isTrigger = true;
        }

        if (collidedThisFrame.contains(it))
        {
            ColManifold& col = collidedThisFrame[it];
            if (!colliderCache.contains(it))
            {
                //Tile stuffs
                if (it.first == it.second)
                {
                    //Collision Stay
                    if (isTrigger)
                    {
                        ev->fireEvent(TriggerColliderEvent(rbA, {}, rbA->GetColTag(), tileCollisions[rbA.GetID()],
                            TriggerColliderEventType::TCOL_STAY));
                    } else
                    {
                        ev->fireEvent(CollisionStayEvent(rbA, {}, rbA->GetColTag(), tileCollisions[rbA.GetID()],
                            col.Normal, col.Depth,col.ContactCount,col.Contact1,col.Contact2));
                    }
                    continue;
                }

                //Collision Stay
                if (isTrigger)
                {
                    ev->fireEvent(TriggerColliderEvent(rbA, rbB, rbA->GetColTag(), rbB->GetColTag(),
                        TriggerColliderEventType::TCOL_STAY));
                } else
                {
                    ev->fireEvent(CollisionStayEvent(rbA, rbB, rbA->GetColTag(), rbB->GetColTag(),
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

            //Tile exist
            if (it.first == it.second)
            {
                //Collision Exit
                if (rbA->isTrigger || rbB->isTrigger)
                {
                    ev->fireEvent(TriggerColliderEvent(rbA, {}, rbA->GetColTag(), tileCollisions[rbA.GetID()],
                        TriggerColliderEventType::TCOL_EXIT));
                    continue;
                }
                ev->fireEvent(CollisionExitEvent(rbA, {}, rbA->GetColTag(), tileCollisions[rbA.GetID()]));
                continue;
            }

            //Collision Exit
            if (rbA->isTrigger || rbB->isTrigger)
            {
                ev->fireEvent(TriggerColliderEvent(rbA, rbB, rbA->GetColTag(), rbB->GetColTag(),
                    TriggerColliderEventType::TCOL_EXIT));
                continue;
            }
            ev->fireEvent(CollisionExitEvent(rbA, rbB, rbA->GetColTag(), rbB->GetColTag()));
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
