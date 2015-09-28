//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

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
    : public SimpleCoClass<IEnum>
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
      CurPos = std::begin(Items);
    }
    
  private:
    std::recursive_mutex Mtx;
    typedef std::list<RefObjQIPtr<IBase>> ItemPool;
    typedef ItemPool::iterator Iterator;
    ItemPool Items;
    Iterator CurPos = std::begin(Items);
  };
  
}

#endif  // !__BOSS_COMMON_ENUM_H__
