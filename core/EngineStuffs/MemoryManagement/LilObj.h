#ifndef LILOBJ_H
#define LILOBJ_H

#include "MemoryPool.h"

template<typename Obj>
class LilObj
{
  public:
  LilObj(MemoryPool<Obj>* memPool, int id)
  {
    itemID = id;
    pool = memPool;
  }
  LilObj() = delete;
  ~LilObj() = default;
  Obj& operator*() {return &pool->GetObjByID(itemID);}
  Obj* operator->() {return pool->GetObjByID(itemID);}
  private:
  MemoryPool<Obj>* pool;
  int itemID;
};

#endif //LILOBJ_H
