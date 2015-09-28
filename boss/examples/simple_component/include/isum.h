//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __SIMPLE_COMPONENT_ICALC_H__
#define __SIMPLE_COMPONENT_ICALC_H__

#include "core/ibase.h"

namespace MyNs
{
  
  struct ISum
    : public Boss::Inherit<Boss::IBase>
  {
    BOSS_DECLARE_IFACEID_HEX(0x805B9772)  // CRC32(MyNs.ICalc)
    
    virtual Boss::RetCode BOSS_CALL CalcSum(int a, int b, int *sum) = 0;
  };
  
}

#endif  // !__SIMPLE_COMPONENT_ICALC_H__
