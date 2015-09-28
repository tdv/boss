//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_PLUGIN_ISERVICE_REGISTRY_H__
#define __BOSS_PLUGIN_ISERVICE_REGISTRY_H__

#include "iservice_info.h"

#include "core/ibase.h"
#include "common/iistream.h"
#include "common/iostream.h"

namespace Boss
{
  
  struct IServiceRegistry
    : public Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID_HEX(0xB64AA402)  // CRC32(Boss.IServiceRegistry)
    
    virtual RetCode BOSS_CALL GetServiceInfo(ClassId clsId, IServiceInfo **info) const = 0;
  };
  
  struct IServiceRegistryCtrl
    : public Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID_HEX(0xFEFC5245)  // CRC32(Boss.IServiceRegistryCtrl)
    
    virtual RetCode BOSS_CALL AddService(IServiceInfo *service) = 0;
    virtual RetCode BOSS_CALL DelService(ServiceId serviceId) = 0;
  };
  
}

#endif  // !__BOSS_PLUGIN_ISERVICE_REGISTRY_H__
