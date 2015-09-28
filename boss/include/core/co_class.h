//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_CORE_CO_CLASS_H__
#define __BOSS_CORE_CO_CLASS_H__

#include "ibase.h"

#include <tuple>

namespace Boss
{
  
  template <typename ... T>
  class SimpleCoClass;

  namespace Private
  {

    struct CoClassBase
    {
      virtual ~CoClassBase() {}
      virtual IBase* GetThisIBase() = 0;
    };

    template <typename T>
    void SetConstructedFlag(T *obj, bool constructed)
    {
      obj->Constructed = constructed;
    }

    template <typename T>
    bool GetConstructedFlag(T *obj)
    {
      return  obj->Constructed;
    }
    
    template <typename>
    struct CoClassFakeBase;
    
    template <typename ... T>
    struct CoClassFakeBase<SimpleCoClass<T ...>>
      : public CoClassBase
    {
    };
    
  }

  template <typename ... T>
  class SimpleCoClass
    : public Private::CoClassFakeBase<SimpleCoClass<T ... >>
    , public T ...
  {
  public:
    typedef std::tuple<T ... > BaseEntities;
    SimpleCoClass()
      : Constructed(false)
    {
    }

    // IBase
    BOSS_DECLARE_IBASE_METHODS()

  private:
    template <typename Y>
    friend void Private::SetConstructedFlag(Y *, bool);
    template <typename Y>
    friend bool Private::GetConstructedFlag(Y *);

    bool Constructed;
  };
  
  template <ClassId ClsId, typename ... T>
  class CoClass
    : public SimpleCoClass<T ...>
  {
  public:
    enum { CoClassId = ClsId  };
  };

}

#endif  // !__BOSS_CORE_CO_CLASS_H__
