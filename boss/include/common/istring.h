//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_COMMON_ISTRING_H__
#define __BOSS_COMMON_ISTRING_H__

#include "../core/ibase.h"
#include "ibuffer.h"

namespace Boss
{
  
  struct IString
    : Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID_HEX(0xA54E1A87)  // CRC32(Boss.IString)
    
    enum
    {
      UnknownStringType,
      AnsiString,
      Utf8String,
      UnicodeString
    };

    virtual RetCode BOSS_CALL GetLength(UInt *length) const = 0;
    virtual RetCode BOSS_CALL GetType(UInt *type) const = 0;
    virtual RetCode BOSS_CALL GetString(UInt type, IBuffer **str) const = 0;
    virtual RetCode BOSS_CALL IsEqual(IString *str) const = 0;
    virtual RetCode BOSS_CALL SetString(UInt type, IBuffer *str) = 0;
    virtual RetCode BOSS_CALL Convert(UInt type) = 0;
    virtual RetCode BOSS_CALL Clear() = 0;
  };
  
}

#endif  // !__BOSS_COMMON_ISTRING_H__
