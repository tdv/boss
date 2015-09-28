//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#include "service_registry.h"
#include "core/error_codes.h"
#include "core/ref_obj_qi_ptr.h"
#include "common/ientity_id.h"
#include "common/entity_id.h"
#include "common/enum.h"
#include "common/enum_helper.h"
#include "plugin/service_info.h"

#include <vector>
#include <list>

namespace Boss
{

  char const ServiceRegistry::RegistryTag[] = "Registry";
  char const ServiceRegistry::ServiceTag[] = "Service";
  char const ServiceRegistry::ServiceIdTag[] = "ServiceId";
  char const ServiceRegistry::ClassIdsTag[] = "ClassIds";
  char const ServiceRegistry::SerializerIdTag[] = "SerializerId";
  char const ServiceRegistry::RemotingIdTag[] = "RemotingId";
  char const ServiceRegistry::TransportIdTag[] = "TransportId";
  char const ServiceRegistry::TransportPropertiesTag[] = "TransportProperties";
  char const ServiceRegistry::ModulePathTag[] = "ModulePath";
  char const ServiceRegistry::ServicesTag[] = "Services";
  
  ServiceRegistry::ServiceRegistry()
  {
  }
  
  ServiceRegistry::~ServiceRegistry()
  {
  }
  
  RetCode BOSS_CALL ServiceRegistry::GetServiceInfo(ClassId clsId, IServiceInfo **info) const
  {
    if (!info || *info)
      return Status::InvalidArgument;
    
    try
    {
      std::lock_guard<std::recursive_mutex> Lock(Mtx);
      auto Iter = Services.find(clsId);
      return Iter != std::end(Services) ? Iter->second.QueryInterface(info) : Status::NotFound;
    }
    catch (...)
    {
    }
    return Status::Fail;
  }
  
  RetCode BOSS_CALL ServiceRegistry::AddService(IServiceInfo *service)
  {
    try
    {
      std::lock_guard<std::recursive_mutex> Lock(Mtx);
      if (!service)
        return Status::InvalidArgument;
      IServiceInfoPtr Info(service);
      ServiceId SrvId = 0;
      RetCode Res = Info->GetServiceId(&SrvId);
      if (Res != Status::Ok)
        return Res;
      RefObjPtr<IEnum> ClassIdsEnum;
      Res = Info->GetClassIds(ClassIdsEnum.GetPPtr());
      if (Res != Status::Ok)
        return Res;
      EnumHelper<IEntityId> ClassIdsEnumHelper(ClassIdsEnum);
      typedef std::vector<ClassId> ClassIdPool;
      ClassIdPool ClassIds;
      if (ClassIdsEnum->Reset() != Status::Ok)
        return Status::Fail;
      for (auto i = ClassIdsEnumHelper.First() ; i.Get() ; i = ClassIdsEnumHelper.Next())
      {
        ClassId ClsId = 0;
        Res = i->GetId(&ClsId);
        if (Res != Status::Ok)
          return Res;
        if (Services.find(ClsId) != std::end(Services))
          return Status::Fail;
        ClassIds.push_back(ClsId);
      }
      auto Pred = [&] (decltype(*ServicePool::const_iterator()) &info)
        {
          ServiceId Id = 0;
          return info.second->GetServiceId(&Id) == Status::Ok && Id == SrvId;
        };
      bool Exists = false;
      for (auto Iter : Services)
      {
        if (Pred(Iter))
        {
          Exists = true;
          break;
        }
      }
      if (Exists)
        return Status::AlreadyExists;
      for (auto const &i : ClassIds)
        Services[i] = Info;
    }
    catch (...)
    {
      return Status::Fail;
    }
    return Status::Ok;
  }
  
  RetCode BOSS_CALL ServiceRegistry::DelService(ServiceId serviceId)
  {
    try
    {
      std::lock_guard<std::recursive_mutex> Lock(Mtx);
      typedef std::vector<ClassId> ClassIdPool;
      ClassIdPool ClassIds;
      for (auto const &i : Services)
      {
        ServiceId SrvId = 0;
        RetCode Res = i.second->GetServiceId(&SrvId);
        if (Res != Status::Ok)
          return Res;
        ClassIds.push_back(i.first);
      }
      for (auto const &i : ClassIds)
        Services.erase(i);
    }
    catch (...)
    {
      return Status::Fail;
    }
    return Status::Ok;
  }
  
  RetCode BOSS_CALL ServiceRegistry::Load(IIStream *stream)
  {
    if (!stream)
      return Status::InvalidArgument;
    std::lock_guard<std::recursive_mutex> Lock(Mtx);
    try
    {
      auto Registry = PropertyBagType::Create(RegistryTag);
      RetCode Code = Registry->Load(stream);
      if (Code != Status::Ok)
        return Code;
      RefObjPtr<IBase> ServiceEnumProp;
      if ((Code = Registry->GetProperty(ServicesTag, ServiceEnumProp.GetPPtr())) != Status::Ok)
        return Code;
      EnumHelper<IPropertyBag> ServiceEnum(ServiceEnumProp);
      ServicePool NewServices;
      for (auto i =ServiceEnum.First() ; i.Get() ; i = ServiceEnum.Next())
      {
        RefObjPtr<IBase> EntId;
        if ((Code = i->GetProperty(ServiceIdTag, EntId.GetPPtr())) != Status::Ok)
          return Code;
        RefObjQIPtr<IEntityId> EntSrvId(EntId);
        if (!EntSrvId.Get())
          return Status::Fail;
        ServiceId SrvId = 0;
        if ((Code = EntSrvId->GetId(&SrvId)) != Status::Ok)
          return Code;
        RefObjPtr<IBase> ClassIdsEnumProp;
        if ((Code = i->GetProperty(ClassIdsTag, ClassIdsEnumProp.GetPPtr())) != Status::Ok)
          return Code;
        RefObjQIPtr<IEnum> ClassIdsEnum(ClassIdsEnumProp);
        if (!ClassIdsEnum.Get())
          return Status::Fail;
        RefObjPtr<IBase> ModulePathProp;
        RefObjPtr<IServiceInfo> ServiceInfo;
        if (i->GetProperty(ModulePathTag, ModulePathProp.GetPPtr()) == Status::Ok)
        {
          RefObjQIPtr<IString> ModulePath(ModulePathProp);
          if (!ModulePath.Get())
            return Status::Fail;
          auto LocalSrvInfo = Base<LocalServiceInfo>::Create();
          LocalSrvInfo->SetServiceId(SrvId);
          LocalSrvInfo->AddCoClassIds(ClassIdsEnum);
          LocalSrvInfo->SetModulePath(ModulePath);
          ServiceInfo = LocalSrvInfo;
        }
        else
        {
          auto RemoteSrvInfo = Base<RemoteServiceInfo>::Create();
          RemoteSrvInfo->SetServiceId(SrvId);
          RemoteSrvInfo->AddCoClassIds(ClassIdsEnum);
          {
            RefObjPtr<IBase> EntId;
            if (i->GetProperty(SerializerIdTag, EntId.GetPPtr()) == Status::Ok)
            {
              RefObjQIPtr<IEntityId> SerializerId(EntId);
              if (!SerializerId.Get())
                return Status::Fail;
              ClassId Id;
              if (SerializerId->GetId(&Id) != Status::Ok)
                return Status::Fail;
              RemoteSrvInfo->SetSerializerId(Id);
            }
            return Status::Fail;
          }
          {
            RefObjPtr<IBase> EntId;
            if (i->GetProperty(RemotingIdTag, EntId.GetPPtr()) == Status::Ok)
            {
              RefObjQIPtr<IEntityId> RemotingId(EntId);
              if (!RemotingId.Get())
                return Status::Fail;
              ClassId Id;
              if (RemotingId->GetId(&Id) != Status::Ok)
                return Status::Fail;
              RemoteSrvInfo->SetRemotingId(Id);
            }
            return Status::Fail;
          }
          {
            RefObjPtr<IBase> EntId;
            if (i->GetProperty(TransportIdTag, EntId.GetPPtr()) == Status::Ok)
            {
              RefObjQIPtr<IEntityId> TransportId(EntId);
              if (!TransportId.Get())
                return Status::Fail;
              ClassId Id;
              if (TransportId->GetId(&Id) != Status::Ok)
                return Status::Fail;
              RemoteSrvInfo->SetTransportId(Id);
            }
            return Status::Fail;
          }
          RefObjPtr<IBase> TransportPropertiesProp;
          if (i->GetProperty(TransportPropertiesTag, TransportPropertiesProp.GetPPtr()) == Status::Ok)
          {
            RefObjQIPtr<IPropertyBag> TransportProperties(TransportPropertiesProp);
            if (!TransportProperties.Get())
              return Status::Fail;
            RemoteSrvInfo->SetProps(TransportProperties);
            ServiceInfo = RemoteSrvInfo;
          }
          else
            return Status::Fail;
        }
        EnumHelper<IEntityId> ClassIdsEnumHelper(ClassIdsEnum);
        for (auto j = ClassIdsEnumHelper.First() ; j.Get() ; j = ClassIdsEnumHelper.Next())
        {
          ClassId Id = 0;
          if ((Code = j->GetId(&Id)) != Status::Ok)
            return Code;
          NewServices[Id] = ServiceInfo;
        }
      }
      std::swap(Services, NewServices);
      return Status::Ok;
    }
    catch (...)
    {
      return Status::Fail;
    }
    return Status::Ok;
  }

  RetCode BOSS_CALL ServiceRegistry::Save(IOStream *stream)
  {
    if (!stream)
      return Status::InvalidArgument;
    try
    {
      std::list<IServiceInfoPtr> ServiceInfoSet;
      {
        std::lock_guard<std::recursive_mutex> Lock(Mtx);
        for (auto const &service : Services)
        {
          bool Continue = false;
          for (auto Info : ServiceInfoSet)
          {
            if (Info.Get() == service.second.Get())
            {
              Continue = true;
              break;
            }
          }
          if (Continue)
            continue;
          ServiceInfoSet.push_back(service.second);
        }
      }
      auto ServiceEnum = Base<Enum>::Create();
      for (auto const &service : ServiceInfoSet)
      {
        RetCode Code = Status::Ok;
        auto ServiceItem = PropertyBagType::Create(ServiceTag);
        ServiceEnum->AddItem(ServiceItem);
        {
          ServiceId SrvId = 0;
          Code = service->GetServiceId(&SrvId);
          if (Code != Status::Ok)
            return Code;
          if ((Code = ServiceItem->SetProperty(ServiceIdTag, Base<EntityId>::Create(SrvId).Get())) != Status::Ok)
            return Code;
        }
        {
          RefObjPtr<IEnum> ClassIds;
          if ((service->GetClassIds(ClassIds.GetPPtr())) != Status::Ok)
            return Code;
          if ((Code = ServiceItem->SetProperty(ClassIdsTag, ClassIds.Get())) != Status::Ok)
            return Code;
        }
        {
          RefObjQIPtr<IRemoteServiceInfo> RemoteService(service);
          if (RemoteService.Get())
          {
            {
              ClassId SerializerId = 0;
              if ((Code = RemoteService->GetSerializerId(&SerializerId)) != Status::Ok ||
                  (Code = ServiceItem->SetProperty(SerializerIdTag, Base<EntityId>::Create(SerializerId).Get())) != Status::Ok)
              {
                return Code;
              }
            }
            {
              ClassId RemotingId = 0;
              if ((Code = RemoteService->GetRemotingId(&RemotingId)) != Status::Ok ||
                  (Code = ServiceItem->SetProperty(RemotingIdTag, Base<EntityId>::Create(RemotingId).Get())) != Status::Ok)
              {
                return Code;
              }
            }
            {
              ClassId TransportId = 0;
              if ((Code = RemoteService->GetTransportId(&TransportId)) != Status::Ok ||
                  (Code = ServiceItem->SetProperty(TransportIdTag, Base<EntityId>::Create(TransportId).Get())) != Status::Ok)
              {
                return Code;
              }
            }
            RefObjPtr<IPropertyBag> TransportProps;
            if ((Code = RemoteService->GetTransportProperties(TransportProps.GetPPtr())) != Status::Ok ||
                (Code = ServiceItem->SetProperty(TransportPropertiesTag, TransportProps.Get())) != Status::Ok)
            {
              return Code;
            }
            continue;
          }
        }
        {
          RefObjQIPtr<ILocalServiceInfo> LocalService(service);
          if (LocalService.Get())
          {
            RefObjPtr<IString> ModulePath;
            if ((Code = LocalService->GetModulePath(ModulePath.GetPPtr())) != Status::Ok ||
                (Code = ServiceItem->SetProperty(ModulePathTag, ModulePath.Get())) != Status::Ok)
            {
              return Code;
            }
            continue;
          }
        }
      }
      auto Registry = PropertyBagType::Create(RegistryTag);
      RetCode Code = Registry->SetProperty(ServicesTag, ServiceEnum.Get());
      if (Code != Status::Ok)
        return Code;
      RefObjQIPtr<ISerializable> Serializable(Registry);
      if (!Serializable.Get())
        return Status::Fail;
      if ((Code = Serializable->Save(stream)) != Status::Ok)
        return Code;
    }
    catch (std::exception const &)
    {
      return Status::Fail;
    }
    return Status::Ok;
  }

}
