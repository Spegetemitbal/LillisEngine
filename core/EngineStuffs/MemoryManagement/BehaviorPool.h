//
// Created by Somed on 12/9/2024.
//

#ifndef BEHAVIORPOOL_H
#define BEHAVIORPOOL_H

#include <iostream>
#include <ostream>

#include "../Behaviors/BehaviorSystem.h"
#include "../Utils/Events/EventSystem.h"
#include "MemoryPool.h"
#include "pch.h"
#include "Utils/Events/InputEvent.h"

struct ForwardingAddress
{
    ForwardingAddress(void* fwd, Behavior* bhv, size_t len)
    {
        forward = fwd;
        origin = bhv;
        objSize = len;
    }
    void* forward;
    Behavior* origin;
    size_t objSize;
};

class BehaviorHandler : public MemoryPool
{
public:

    BehaviorHandler()
    {
        stackSize = 1024 * 10;
        mPool = DBG_NEW char[stackSize];
        mHead = mPool;
    }

    explicit BehaviorHandler(size_t sz)
    {
        stackSize = 1024 * sz;
        mPool = DBG_NEW char[stackSize];
        mHead = mPool;
    }

    ~BehaviorHandler()
    {
        for (auto & i : poolDir)
        {
            i->DeallocateBehavior();
        }
    }

    LilObj<Behavior> CreateBehavior(const std::string& typeID)
    {
        BehaviorData B = BehaviorSystem::GetBehavior(typeID);
        try
        {
            size_t sizeToAllocate = B.byteSize;
            if (mHead + sizeToAllocate > mPool + stackSize)
            {
                ResizePool();
            }

            Behavior* b = B.generator(mHead);
            poolDir.push_back(b);
            poolDir.back()->SetActive(true);
            objMap[poolDir.back()->GetID()] = poolDir.back();
            mCount++;
            mHead += sizeToAllocate;
            return {this, poolDir.back()->GetID()};
        } catch(...)
        {
            std::cerr << "Failed to allocate Behavior" << std::endl;
            return {};
        }
    }

    void ClearPool()
    {
        mHead = mPool;
        objMap.clear();
        poolDir.clear();
        UpdateEventPointers();
    }

    void CompactPool(int active) override
    {
        if (mCount == 0)
        {
            return;
        }

        if (active / mCount > 0.5)
        {
            EventSystem* ev = EventSystem::getInstance();

            char* free = mPool;

            std::vector<ForwardingAddress> addresses = std::vector<ForwardingAddress>();

            //Load over everything.
            for (auto & index : poolDir)
            {
                BehaviorData B = BehaviorSystem::GetBehavior(index->GetName());
                if (index->GetActive())
                {
                    addresses.emplace_back(free, index, B.byteSize);
                } else
                {
                    index->DeallocateBehavior();
                }
                free += B.byteSize;
            }

            poolDir.clear();

            //Relocate
            for (auto address : addresses)
            {
                if (address.forward != address.origin)
                {
                    ev->removeListenerFromAllEvents(address.origin);
                    poolDir.push_back((Behavior*)address.forward);
                    std::memmove(address.forward, address.origin, address.objSize);
                }
            }

            //refill map.
            objMap.clear();
            for (int i = 0; i < poolDir.size(); i++)
            {
                objMap.emplace(poolDir[i]->GetID(), poolDir[i]);
            }

            UpdateEventPointers();
            mCount = poolDir.size();
        }
    }

    ActiveTracker<Behavior*> getPool() {return {poolDir, (unsigned int)poolDir.size()};}

protected:

    std::vector<Behavior*> poolDir;

    void ResizePool() override
    {
        EventSystem* ev = EventSystem::getInstance();
        for (auto & i : poolDir)
        {
            ev->removeListenerFromAllEvents(i);
        }

        //Clear previous data (that isn't needed)
        objMap.clear();
        poolDir.resize(poolDir.size() * 2);
        stackSize *= 2;
        size_t numObjects = poolDir.size();
        vector tempDir(poolDir);
        poolDir.clear();

        //Make new pool to copy things over to.
        char* tempPool = DBG_NEW char[stackSize];

        //Copy all active objects over from previous pool to new pool.
        char* tempHead = tempPool;
        for (int i = 0; i < tempDir.size(); i++)
        {
            //Find a safer way to do this!
            if (tempDir[i]->GetActive())
            {
                BehaviorData B = BehaviorSystem::GetBehavior(tempDir[i]->GetName());
                std::memmove(tempHead, tempDir[i], B.byteSize);
                poolDir.push_back(reinterpret_cast<Behavior*>(tempHead));
                tempHead += B.byteSize;
            } else
            {
                tempDir[i]->DeallocateBehavior();
            }
        }

        //Final swap over.
        delete[] mPool;
        mPool = tempPool;
        mHead = tempHead;
        //Reload Map
        for (int i = 0; i < poolDir.size(); i++)
        {
            objMap.emplace(poolDir[i]->GetID(), poolDir[i]);
        }
        mCount = poolDir.size();
        UpdateEventPointers();
    }
private:

    void UpdateEventPointers()
    {
        EventSystem* ev = EventSystem::getInstance();

        for (int i = 0; i < poolDir.size(); i++)
        {
            poolDir[i]->LoadListeners();
        }
    }

    size_t stackSize;
    char* mHead;
};

#endif //BEHAVIORPOOL_H
