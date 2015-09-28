//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_COMMON_ISERVICE_LOCATOR_H__
#define __BOSS_COMMON_ISERVICE_LOCATOR_H__

#include "../core/ibase.h"


namespace Boss
{

  struct IServiceLocator
    : public Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID_HEX(0x29D4D8A5)  // CRC32(Boss.IServiceLocator)

    virtual RetCode BOSS_CALL GetService(UInt serviceId, IBase **service) const = 0;

    virtual RetCode BOSS_CALL AddService(UInt serviceId, IBase *service) = 0;
    virtual RetCode BOSS_CALL DelService(UInt serviceId) = 0;
  };

}

#endif  // !__BOSS_COMMON_ISERVICE_LOCATOR_H__
