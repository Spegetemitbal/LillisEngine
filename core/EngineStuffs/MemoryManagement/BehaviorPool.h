//
// Created by Somed on 12/9/2024.
//

#ifndef BEHAVIORPOOL_H
#define BEHAVIORPOOL_H

#include <iostream>
#include <ostream>

#include "../Behaviors/Behavior.h"
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

    template<typename B>
    LilObj<B> CreateBehavior()
    {
        try
        {
            size_t sizeToAllocate = sizeof(B);
            if (mHead + sizeToAllocate > mPool + stackSize)
            {
                ResizePool();
            }

            B* b = AllocateObj<B>(mHead);
            poolDir.push_back(b);
            objMap[poolDir.back()->GetID()] = poolDir.back();
            mCount++;
            numActive++;
            mHead += sizeToAllocate;
            return {this, poolDir.back()->GetID()};
        } catch(...)
        {
            std::cerr << "Failed to allocate Behavior" << std::endl;
            return LilObj<B>();
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
            --numActive;
        }

        if (numActive / mCount < 0.5)
        {
            CompactPool();
        }
    }


protected:
    void CompactPool() override
    {
        //TODO Threaded compaction... or something
    }

    void ResizePool() override
    {
        //TODO Should be similar to the others, but differing compaction sizes.
    }
private:
    size_t stackSize;
    char* mHead;
};


#endif //BEHAVIORPOOL_H
