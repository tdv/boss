//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_PLUGIN_SERVICE_REGISTRY_H__
#define __BOSS_PLUGIN_SERVICE_REGISTRY_H__

#include "core/co_class.h"
#include "core/base.h"

#include "plugin/iservice_registry.h"
#include "plugin/service_ids.h"
#include "core/ref_obj_ptr.h"
#include "common/iserializable.h"
#include "common/xml_property_bag.h"

#include <mutex>
#include <unordered_map>

namespace Boss
{
  
  class ServiceRegistry
    : public CoClass
        <
          Service::Id::ServiceRegistry,
          IServiceRegistry,
          IServiceRegistryCtrl,
          ISerializable
        >
  {
  public:
    ServiceRegistry();
    virtual ~ServiceRegistry();
    
  private:
    mutable std::recursive_mutex Mtx;

    typedef RefObjPtr<IServiceInfo> IServiceInfoPtr;
    typedef std::unordered_map<ClassId, IServiceInfoPtr> ServicePool;
    mutable ServicePool Services;
    
    static char const RegistryTag[];
    static char const ServiceTag[];
    static char const ServiceIdTag[];
    static char const ClassIdsTag[];
    static char const SerializerIdTag[];
    static char const RemotingIdTag[];
    static char const TransportIdTag[];
    static char const TransportPropertiesTag[];
    static char const ModulePathTag[];
    static char const ServicesTag[];
    
    typedef Base<XmlPropertyBag> PropertyBagType;
        
    // IServiceRegistry
    virtual RetCode BOSS_CALL GetServiceInfo(ClassId clsId, IServiceInfo **info) const;
    
    // IServiceRegistryCtrl
    virtual RetCode BOSS_CALL AddService(IServiceInfo *service);
    virtual RetCode BOSS_CALL DelService(ServiceId serviceId);
    
    // ISerializable
    virtual RetCode BOSS_CALL Load(IIStream *stream);
    virtual RetCode BOSS_CALL Save(IOStream *stream);
  };
  
}

#endif  // !__BOSS_PLUGIN_SERVICE_REGISTRY_H__
