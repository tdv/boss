//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_COMMON_IENUM_H__
#define __BOSS_COMMON_IENUM_H__

#include "../core/ibase.h"

namespace Boss
{
  
  struct IEnum
    : Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID_HEX(0xE2CD2D56)  // CRC32(Boss.IEnum)
    
    virtual RetCode Next(IBase **item) = 0;
    virtual RetCode Reset() = 0;
  };
  
}

#endif  // !__BOSS_COMMON_IENUM_H__
