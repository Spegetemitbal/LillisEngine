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
    Obj* GetObjByID(int id)
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

    Obj* AllocateObj(char* base)
    {
        //size_t sizeToAllocate = sizeof(Obj);
        return new (base)Obj();
    }

    std::map<int , Obj*> objMap;

    virtual void SortPool() {};
    virtual void ResizePool() = 0;
    char* mPool = nullptr;
};

#endif //MEMORYPOOL_H
