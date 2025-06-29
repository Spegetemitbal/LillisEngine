#ifndef LILOBJ_H
#define LILOBJ_H

#include "MemoryPool.h"

template<typename Obj>
class LilObj
{
  public:
  LilObj(MemoryPool* memPool, unsigned int id)
  {
    itemID = id;
    pool = memPool;
  }
  LilObj()
  {
    itemID = 0;
    pool = nullptr;
  };
  ~LilObj() = default;
  Obj& operator*()
  {
    if (itemID > 0)
    {
      return &pool->GetObjByID<Obj>(itemID);
    } else
    {
      return nullptr;
    }
  }
  Obj* operator->()
  {
    if (itemID > 0)
    {
      return pool->GetObjByID<Obj>(itemID);
    } else
    {
      return nullptr;
    }
  }

  Obj* GetRawPtr()
  {
    return pool->GetObjByID<Obj>(itemID);
  }

  bool Exists() const
  {
    if (itemID == 0 || pool == nullptr)
    {
      return false;
    }
    if (pool->GetObjByID<Obj>(itemID) == nullptr)
    {
      return false;
    }

    return true;
  }

  MemoryPool* GetPool() const {return pool;}
  unsigned int GetID() const {return itemID;}

  bool operator<(const LilObj& obj)
  {
    if (itemID < obj.GetID())
    {
      return true;
    } else
    {
      return false;
    }
  }

  bool operator==(const LilObj& obj)
  {
    if (obj.GetID() == itemID)
    {
      return true;
    } else
    {
      return false;
    }
  }

  private:
  MemoryPool* pool;
  //All item IDs start at 1
  unsigned int itemID;
};

#endif //LILOBJ_H
