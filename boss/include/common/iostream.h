//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_COMMON_IOSTREAM_H__
#define __BOSS_COMMON_IOSTREAM_H__

#include "../core/ibase.h"

namespace Boss
{
  
  struct IOStream
    : Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID_HEX(0x7AF8049C)  // CRC32(Boss.IOStream)

    virtual RetCode BOSS_CALL Write(void const *data, UInt bytes) = 0;
  };
  
}

#endif  // !__BOSS_COMMON_IOSTREAM_H__
