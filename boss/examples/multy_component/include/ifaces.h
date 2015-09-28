//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     03.05.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __MULTY_COMPONENT_IFACES_H__
#define __MULTY_COMPONENT_IFACES_H__

#include "core/ibase.h"

namespace MyNs
{
  
  struct IFace1
    : public Boss::Inherit<Boss::IBase>
  {
    BOSS_DECLARE_IFACEID_HEX(0xA8B1CBFB)  // CRC32(MyNs.IFace1)

    virtual Boss::RetCode BOSS_CALL Method() = 0;
  };
  
  struct IFace2
    : public Boss::Inherit<Boss::IBase>
  {
    BOSS_DECLARE_IFACEID_HEX(0x31B89A41)  // CRC32(MyNs.IFace2)

    virtual Boss::RetCode BOSS_CALL Method() = 0;
  };
  
}

#endif  // !__MULTY_COMPONENT_IFACES_H__
