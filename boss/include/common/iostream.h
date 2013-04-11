#ifndef __BOSS_COMMON_IOSTREAM_H__
#define __BOSS_COMMON_IOSTREAM_H__

#include "../core/ibase.h"

namespace Boss
{
  
  struct IOStream
    : Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID("Boss.IOStream")
    virtual RetCode BOSS_CALL Write(void const *data, UInt bytes) = 0;
  };
  
}

#endif  // !__BOSS_COMMON_IOSTREAM_H__
