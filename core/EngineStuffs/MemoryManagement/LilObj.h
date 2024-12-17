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

  bool Exists()
  {
    if (itemID == 0)
    {
      return false;
    }
    return true;
  }
  private:
  MemoryPool* pool;
  //All item IDs start at 1
  unsigned int itemID;
};

#endif //LILOBJ_H
