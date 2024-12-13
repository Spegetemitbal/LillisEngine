//
// Created by Somed on 12/9/2024.
//

#ifndef BEHAVIORPOOL_H
#define BEHAVIORPOOL_H

#include <iostream>
#include <ostream>

#include "../Behaviors/BehaviorSystem.h"
#include "MemoryPool.h"
#include "pch.h"

class BehaviorHandler : public MemoryPool<Behavior>
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

    //Called by user createcomponent
    template<typename B>
    LilObj<B> CreateBehavior(std::string behvID)
    {
        try
        {
            Behavior* behv = BehaviorSystem::getInstance()->behaviorMap[behvID];
            size_t sizeToAllocate = sizeof(*behv);
            if (mHead + sizeToAllocate > mPool + stackSize)
            {
                ResizePool();
            }
            void* ptr = memcpy(behv, mHead, sizeToAllocate);
            B* rPtr = static_cast<B*>(ptr);
            poolDir.push_back(rPtr);
            objMap[poolDir.back()->GetID()] = poolDir.back();
            mCount++;
            mHead += sizeToAllocate;
            return {this, poolDir.back()->GetID()};
        } catch(...)
        {
            std::cerr << "Failed to allocate Behavior" << std::endl;
            return LilObj<B>();
        }
    }

    //Called by sceneloader
    LilObj<Behavior> CreateBehaviorGeneric(std::string behvID)
    {
        try
        {
            Behavior* behv = BehaviorSystem::getInstance()->behaviorMap[behvID];
            size_t sizeToAllocate = sizeof(*behv);
            if (mHead + sizeToAllocate > mPool + stackSize)
            {
                ResizePool();
            }

            void* ptr = memcpy(behv, mHead, sizeToAllocate);
            Behavior* rPtr = static_cast<Behavior*>(ptr);
            poolDir.push_back(rPtr);
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

    //Technically doesn't destroy the data yet but marks it as garbage.
    void DestroyBehavior(Behavior* Bhvr)
    {
        auto f = std::find(poolDir.begin(), poolDir.end(), Bhvr);
        if (f != poolDir.end())
        {
            size_t index = distance(poolDir.begin(), f);
            objMap.erase(Bhvr->GetID());
            poolDir.erase(f);
        }
    }

    void CompactPool(int active) override
    {
        //TODO LISP2 compaction... or something
    }

protected:

    void ResizePool() override
    {
        //TODO Should be similar to the others, but differing compaction sizes.
    }
private:
    size_t stackSize;
    char* mHead;
};


#endif //BEHAVIORPOOL_H
