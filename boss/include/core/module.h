//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     03.05.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __CORE_MODULE_H__
#define __CORE_MODULE_H__

#include "private/module_ref_counter_impl.h"

#include <atomic>

namespace Boss
{

  namespace Private
  {

    namespace
    {

      std::atomic<UInt>& GetModuleCounter()
      {
        static std::atomic<UInt> Counter;
        return Counter;
      }
      
    }
    
    void ModuleCounterImpl::AddRef()
    {
      GetModuleCounter().fetch_add(1, std::memory_order_relaxed);
    }

    void ModuleCounterImpl::Release()
    {
      GetModuleCounter().fetch_sub(1, std::memory_order_relaxed);
    }

    UInt ModuleCounterImpl::GetCounter()
    {
      return GetModuleCounter();
    }

  }

}

#endif  // !__CORE_MODULE_H__
