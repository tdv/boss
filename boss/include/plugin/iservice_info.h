//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_PLUGIN_ISERVICE_INFO_H__
#define __BOSS_PLUGIN_ISERVICE_INFO_H__

#include "../core/ibase.h"
#include "../common/ienum.h"
#include "../common/istring.h"
#include "../common/iproperty_bag.h"

namespace Boss
{
  
  struct IServiceInfo
    : public Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID_HEX(0x7BD8C883)  // CRC32(Boss.IServiceInfo)
    
    virtual RetCode BOSS_CALL GetServiceId(ServiceId *serviceId) const = 0;
    virtual RetCode BOSS_CALL GetClassIds(IEnum **ids) const = 0;
  };
  
  struct ILocalServiceInfo
    : public Inherit<IServiceInfo>
  {
    BOSS_DECLARE_IFACEID_HEX(0xD893276E)  // CRC32(Boss.ILocalServiceInfo)
    
    virtual RetCode BOSS_CALL GetModulePath(IString **path) const = 0;
  };
  
  struct IRemoteServiceInfo
    : public Inherit<IServiceInfo>
  {
    BOSS_DECLARE_IFACEID_HEX(0x5D155638)  // CRC32(Boss.IRemoteServiceInfo)
    
    virtual RetCode BOSS_CALL GetSerializerId(ClassId *id) const = 0;
    virtual RetCode BOSS_CALL GetRemotingId(ClassId *id) const = 0;
    virtual RetCode BOSS_CALL GetTransportId(ClassId *id) const = 0;
    virtual RetCode BOSS_CALL GetTransportProperties(IPropertyBag **props) const = 0;
  };

  
}

#endif  // !__BOSS_PLUGIN_ISERVICE_INFO_H__
