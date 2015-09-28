//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __SIMPLE_COMPONENT_CALC_H__
#define __SIMPLE_COMPONENT_CALC_H__

#include "isum.h"
#include "class_ids.h"
#include "core/co_class.h"

namespace MyNs
{

  class Sum
    : public Boss::CoClass
          <
            Service::Id::Sum,
            ISum
          >
  {
  public:
    // ISum
    virtual Boss::RetCode BOSS_CALL CalcSum(int a, int b, int *sum);
  };
  
}

#endif  // !__SIMPLE_COMPONENT_CALC_H__
