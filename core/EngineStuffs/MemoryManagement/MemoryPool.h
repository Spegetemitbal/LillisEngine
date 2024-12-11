//
// Created by Somed on 12/4/2024.
//

#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H
#include <map>
#include <vector>

template<typename T>
class ActiveTracker
{
public:

    ActiveTracker() = delete;
    ~ActiveTracker() = default;

    ActiveTracker(vector<T> v) {data = v;}

    T operator[](int idx)
    {
        try
        {
            if (!data[idx]->GetActive())
            {
                counter++;
            }
            return data[idx];
        } catch (...)
        {
            std::cerr << "Invalid Tracker type" << std::endl;
            return nullptr;
        }
    }

    int GetNumActive() const {return counter;}

private:
    vector<T> data;
    int counter = 0;
};

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

    ActiveTracker<Obj*> getPool() {return {poolDir};}

    virtual void CompactPool(int active) = 0;
protected:

    std::vector<Obj*> poolDir;

    template<typename t>
    t* AllocateObj(char* base)
    {
        //size_t sizeToAllocate = sizeof(Obj);
        return new (base)t();
    }

    std::map<unsigned int , Obj*> objMap;

    virtual void ResizePool() = 0;
    //unsigned int numActive = 0;
    unsigned int mCount = 0;
    char* mPool = nullptr;
};

#endif //MEMORYPOOL_H
