#ifndef __BOSS_COMMON_ENUM_H__
#define __BOSS_COMMON_ENUM_H__

#include "../core/co_class.h"
#include "../core/ref_obj_qi_ptr.h"
#include "../core/error_codes.h"
#include "ienum.h"

#include <list>
#include <mutex>

namespace Boss
{
  
  class Enum
    : public CoClass<Crc32("Boss.Enum"), IEnum>
  {
  public:
    // IEnum
    virtual RetCode Next(IBase **item)
    {
      if (!item || *item)
        return Status::InvalidArgument;
      try
      {
        std::lock_guard<std::recursive_mutex> Lock(Mtx);
        if (CurPos == std::end(Items))
          return Status::False;
        return (CurPos++)->QueryInterface(item);
      }
      catch (std::exception const &)
      {
        return Status::Fail;
      }
    }
    virtual RetCode Reset()
    {
      try
      {
        std::lock_guard<std::recursive_mutex> Lock(Mtx);
        CurPos = Items.begin();
      }
      catch (std::exception const &)
      {
        return Status::Fail;
      }
      return Status::Ok;
    }
    
    template <typename T>
    void AddItem(RefObjPtr<T> item)
    {
      Items.push_back(item);
      CurPos = Items.begin();
    }
    
  private:
    std::recursive_mutex Mtx;
    typedef std::list<RefObjQIPtr<IBase>> ItemPool;
    typedef ItemPool::iterator Iterator;
    ItemPool Items;
    Iterator CurPos;
  };
  
}

#endif  // !__BOSS_COMMON_ENUM_H__
