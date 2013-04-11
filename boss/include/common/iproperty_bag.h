#ifndef __BOSS_COMMON_IPROPERTY_BAG_H__
#define __BOSS_COMMON_IPROPERTY_BAG_H__

#include "../core/ibase.h"
#include "ienum.h"

namespace Boss
{
  
  struct IPropertyBag
    : Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID("Boss.IPropertyBag")
    
    virtual RetCode BOSS_CALL GetProperty(char const *name, IBase **value) const = 0;
    virtual RetCode BOSS_CALL SetProperty(char const *name, IBase *value) = 0;
    virtual RetCode BOSS_CALL EnumProperties(IEnum **props) = 0;
  };
  
}

#endif  // !__BOSS_COMMON_IPROPERTY_BAG_H__
