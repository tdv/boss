#ifndef __BOSS_COMMON_IENUM_H__
#define __BOSS_COMMON_IENUM_H__

#include "../core/ibase.h"

namespace Boss
{
  
  struct IEnum
    : Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID("Boss.IEnum")
    virtual RetCode Next(IBase **item) = 0;
    virtual RetCode Reset() = 0;
  };
  
}

#endif  // !__BOSS_COMMON_IENUM_H__
