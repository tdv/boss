//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_CORE_PRIVATE_MODULE_REF_COUNTER_IMPL_H__
#define __BOSS_CORE_PRIVATE_MODULE_REF_COUNTER_IMPL_H__

#include "../core_types.h"

#include <atomic>

namespace Boss
{

  namespace Private
  {
 
    class ModuleCounterImpl
    {
    public:
      static void AddRef();
      static void Release();
      static UInt GetCounter();
    };

  }

}

#endif  // !__BOSS_CORE_PRIVATE_MODULE_REF_COUNTER_IMPL_H__
