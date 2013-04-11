#ifndef __BOSS_COMMON_ISTRING_H__
#define __BOSS_COMMON_ISTRING_H__

#include "../core/ibase.h"
#include "ibuffer.h"

namespace Boss
{
  
  struct IString
    : Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID("Boss.IString")
    
    enum
    {
      AnsiString,
      Utf8String,
      UnicodeString
    };
    
    virtual RetCode BOSS_CALL GetString(UInt type, IBuffer **str) const = 0;
  };
  
}

#endif  // !__BOSS_COMMON_ISTRING_H__
