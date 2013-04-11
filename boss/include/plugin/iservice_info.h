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
    BOSS_DECLARE_IFACEID("Boss.IServiceInfo")
    
    virtual RetCode BOSS_CALL GetServiceId(ServiceId *serviceId) const = 0;
    virtual RetCode BOSS_CALL GetClassIds(IEnum **ids) const = 0;
  };
  
  struct ILocalServiceInfo
    : public Inherit<IServiceInfo>
  {
    BOSS_DECLARE_IFACEID("Boss.ILocalServiceInfo")
    
    virtual RetCode BOSS_CALL GetModulePath(IString **path) const = 0;
  };
  
  struct IRemoteServiceInfo
    : public Inherit<IServiceInfo>
  {
    BOSS_DECLARE_IFACEID("Boss.IRemoteServiceInfo")
    
    virtual RetCode BOSS_CALL GetProperties(IPropertyBag **props) const = 0;
  };

  
}

#endif  // !__BOSS_PLUGIN_ISERVICE_INFO_H__
