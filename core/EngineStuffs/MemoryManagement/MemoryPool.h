//
// Created by Somed on 12/4/2024.
//

#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H
#include <map>
#include <vector>

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
            //std::cout << "Invalid type or ID";
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

template<typename T>
class ActiveTracker
{
public:

    ActiveTracker() = delete;
    ~ActiveTracker() = default;

    ActiveTracker(std::vector<T> v, unsigned int activeLine, MemoryPool* pool)
    {
        data = v;
        _activeLine = activeLine;
        mPool = pool;
    }

    T operator[](int idx)
    {
        try
        {
            return data[idx];
        } catch (...)
        {
            std::cerr << "Invalid Tracker type" << std::endl;
            return nullptr;
        }
    }

    size_t size() {return data.size();}

    int GetNumInactive() const
    {
        int result = 0;
        for (int i = 0; i < _activeLine; i++)
        {
            try
            {
                if (!data[i]->GetActive())
                {
                    result++;
                }
            } catch (...)
            {
                std::cerr << "Invalid Tracker type" << std::endl;
                throw;
            }
        }
        return result;
    }

    MemoryPool* getMPool()
    {
        return mPool;
    }

private:
    std::vector<T> data;
    unsigned int _activeLine;
    MemoryPool* mPool = nullptr;
};

#endif //MEMORYPOOL_H
