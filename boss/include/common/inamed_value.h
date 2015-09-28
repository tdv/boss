//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_COMMON_INAMED_VALUE_H__
#define __BOSS_COMMON_INAMED_VALUE_H__

#include "../core/ibase.h"
#include "istring.h"

namespace Boss
{
  
  struct INamedValue
    : public Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID_HEX(0x4E1C0E57)  // CRC32(Boss.INamedValue)
    
    virtual RetCode BOSS_CALL GetName(IString **name) const = 0;
    virtual RetCode BOSS_CALL GetValue(IBase **value) const = 0;
  };
  
}

#endif  // !__BOSS_COMMON_INAMED_VALUE_H__
