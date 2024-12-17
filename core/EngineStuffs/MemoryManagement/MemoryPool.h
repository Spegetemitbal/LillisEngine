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
class MemoryPool
{
public:
    MemoryPool() = default;

    virtual ~MemoryPool()
    {
        delete[] mPool;
    };

    template<typename T>
    T* GetObjByID(unsigned int id)
    {
        try
        {
            return (T*)objMap[id];
        } catch(...)
        {
            std::cout << "Invalid type or ID";
            return nullptr;
        }
    }

    virtual void CompactPool(int active) = 0;
protected:

    template<typename T>
    T* AllocateObj(char* base)
    {
        //size_t sizeToAllocate = sizeof(Obj);
        return new (base)T();
    }

    std::map<unsigned int , void*> objMap;

    virtual void ResizePool() = 0;
    //unsigned int numActive = 0;
    unsigned int mCount = 0;
    char* mPool = nullptr;
};

#endif //MEMORYPOOL_H
