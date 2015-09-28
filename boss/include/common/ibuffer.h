//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_COMMON_IBUFFER_H__
#define __BOSS_COMMON_IBUFFER_H__

#include "../core/ibase.h"

namespace Boss
{
  
  struct IBuffer
    : public Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID_HEX(0x0AAA58DC)  // CRC32(Boss.IBuffer)
    
    virtual RetCode BOSS_CALL GetData(void const **data) const = 0;
    virtual RetCode BOSS_CALL GetSize(UInt *size) const = 0;
  };
  
}

#endif  // !__BOSS_COMMON_IBUFFER_H__
