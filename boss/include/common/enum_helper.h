//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     04.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_COMMON_ENUM_HELPER_H__
#define __BOSS_COMMON_ENUM_HELPER_H__

#include "../core/exceptions.h"
#include "../core/ref_obj_ptr.h"
#include "../core/error_codes.h"
#include "ienum.h"

#include <utility>

namespace Boss
{
  BOSS_DECLARE_RUNTIME_EXCEPTION(EnumHelper)

  template <typename T>
  class EnumHelper final
  {
  public:
    typedef RefObjQIPtr<IEnum> IEnumPtr;
    typedef RefObjPtr<IBase> IBasePtr;

    EnumHelper(EnumHelper const &) = delete;
    EnumHelper& operator = (EnumHelper const &) = delete;
    
    EnumHelper(IEnum *enumerator)
      : Enumerator(enumerator)
    {
      if (!Enumerator.Get())
        throw EnumHelperException("Empty IEnum pointer");
    }
    EnumHelper(IEnumPtr enumerator)
      : Enumerator(enumerator)
    {
      if (!Enumerator.Get())
        throw EnumHelperException("Empty IEnum pointer");
    }

    RefObjPtr<T> First()
    {
      ResetInternal();
      return std::move(Next());
    }
    RefObjPtr<T> Next()
    {
      RefObjQIPtr<T> Ret;
      for (IBasePtr i ; (i = NextInternal()).Get() ; )
      {
        if ((Ret = std::move(i)).Get())
          break;
      }
      return std::move(Ret);
    }

  private:
    IEnumPtr Enumerator;
    
    void ResetInternal()
    {
      if (Enumerator->Reset() != Status::Ok)
        throw EnumHelperException("Failed to reset enumerator.");
    }
    IBasePtr NextInternal()
    {
      IBasePtr Ret;
      RetCode Code = Enumerator->Next(Ret.GetPPtr());
      if (Code != Status::Ok && Code != Status::False)
        throw EnumHelperException("Failed to get next item.");
      return std::move(Ret);
    }
  };

}
#endif  // ! __BOSS_COMMON_ENUM_HELPER_H__
