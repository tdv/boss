//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_CORE_PRIVATE_MODULE_REF_COUNTER_H__
#define __BOSS_CORE_PRIVATE_MODULE_REF_COUNTER_H__

#include "../core_types.h"
#include "module_ref_counter_impl.h"

namespace Boss
{

  namespace Private
  {
 
    class ModuleCounter
    {
    public:
      static void AddRef()
      {
        ModuleCounterImpl::AddRef();
      }
      static void Release()
      {
        ModuleCounterImpl::Release();
      }
      static UInt GetCounter()
      {
        return ModuleCounterImpl::GetCounter();
      }

      class ScopedLock
      {
      public:
        ScopedLock(ScopedLock const &) = delete;
        ScopedLock(ScopedLock &&) = delete;
        ScopedLock operator = (ScopedLock const &) = delete;
        ScopedLock operator = (ScopedLock &&) = delete;

        ScopedLock()
        {
          ModuleCounter::AddRef();
        }
        ~ScopedLock()
        {
          ModuleCounter::Release();
        }
      };
    };

  }

}

#endif  // !__BOSS_CORE_PRIVATE_MODULE_REF_COUNTER_H__
