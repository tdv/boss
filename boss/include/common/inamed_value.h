#ifndef __BOSS_COMMON_INAMED_VALUE_H__
#define __BOSS_COMMON_INAMED_VALUE_H__

#include "../core/ibase.h"
#include "istring.h"

namespace Boss
{
  
  struct INamedValue
    : public Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID("Boss.INamedValue")
    
    virtual RetCode BOSS_CALL GetName(IString **name) const = 0;
    virtual RetCode BOSS_CALL GetValue(IBase **value) const = 0;
  };
  
}

#endif  // !__BOSS_COMMON_INAMED_VALUE_H__
