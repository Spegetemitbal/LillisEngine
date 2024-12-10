//
// Created by Somed on 12/4/2024.
//

#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H
#include <map>
#include <vector>

template<typename Obj>
class MemoryPool
{
public:
    MemoryPool() = default;

    virtual ~MemoryPool()
    {
        delete[] mPool;
        poolDir.clear();
    };

    std::vector<Obj*> poolDir;
    Obj* GetObjByID(unsigned int id)
    {
        try
        {
            return objMap[id];
        } catch(...)
        {
            return nullptr;
        }
    }
protected:

    template<typename t>
    t* AllocateObj(char* base)
    {
        //size_t sizeToAllocate = sizeof(Obj);
        return new (base)t();
    }

    std::map<unsigned int , Obj*> objMap;

    virtual void CompactPool() {};
    virtual void ResizePool() = 0;
    unsigned int numActive = 0;
    unsigned int mCount = 0;
    char* mPool = nullptr;
};

#endif //MEMORYPOOL_H
