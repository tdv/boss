//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_COMMON_IPROPERTY_BAG_H__
#define __BOSS_COMMON_IPROPERTY_BAG_H__

#include "../core/ibase.h"
#include "ienum.h"

namespace Boss
{
  
  struct IPropertyBag
    : Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID_HEX(0x6EE4C41A)  // CRC32(Boss.IPropertyBag)
    
    virtual RetCode BOSS_CALL GetProperty(char const *name, IBase **value) const = 0;
    virtual RetCode BOSS_CALL SetProperty(char const *name, IBase *value) = 0;
    virtual RetCode BOSS_CALL EnumProperties(IEnum **props) = 0;
  };
  
}

#endif  // !__BOSS_COMMON_IPROPERTY_BAG_H__
