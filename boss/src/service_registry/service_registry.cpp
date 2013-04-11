#include "service_registry.h"
#include "core/error_codes.h"
#include "core/ref_obj_qi_ptr.h"
#include "common/ientity_id.h"
#include "common/entity_id.h"
#include "common/enum.h"
#include "plugin/service_info.h"

#include <algorithm>
#include <vector>
#include <list>

namespace Boss
{

  char const ServiceRegistry::RegistryTag[] = "Registry";
  char const ServiceRegistry::ServiceTag[] = "Service";
  char const ServiceRegistry::ServiceIdTag[] = "ServiceId";
  char const ServiceRegistry::ClassIdsTag[] = "ClassIds";
  char const ServiceRegistry::RemotePropertiesTag[] = "RemoteProperties";
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
      typedef std::vector<ClassId> ClassIdPool;
      ClassIdPool ClassIds;
      if (ClassIdsEnum->Reset() != Status::Ok)
        return Status::Fail;
      for (RefObjPtr<IBase> i ; ClassIdsEnum->Next(i.GetPPtr()) == Status::Ok ; i.Release())
      {
        RefObjQIPtr<IEntityId> Id(i);
        if (!Id.Get())
          return Status::Fail;
        ClassId ClsId = 0;
        Res = Id->GetId(&ClsId);
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
      auto Iter = std::find_if(std::begin(Services), std::end(Services), Pred);
      if (Iter != std::end(Services))
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
      RefObjQIPtr<IEnum> ServiceEnum(ServiceEnumProp);
      if (!ServiceEnum.Get())
        return Status::Fail;
      ServicePool NewServices;
      for (RefObjPtr<IBase> i ; ServiceEnum->Next(i.GetPPtr()) == Status::Ok ; i.Release())
      {
        RefObjQIPtr<IPropertyBag> ServiceItem(i);
        if (!ServiceItem.Get())
          return Status::Fail;
        RefObjPtr<IBase> EntId;
        if ((Code = ServiceItem->GetProperty(ServiceIdTag, EntId.GetPPtr())) != Status::Ok)
          return Code;
        RefObjQIPtr<IEntityId> EntSrvId(EntId);
        if (!EntSrvId.Get())
          return Status::Fail;
        ServiceId SrvId = 0;
        if ((Code = EntSrvId->GetId(&SrvId)) != Status::Ok)
          return Code;
        RefObjPtr<IBase> ClassIdsEnumProp;
        if ((Code = ServiceItem->GetProperty(ClassIdsTag, ClassIdsEnumProp.GetPPtr())) != Status::Ok)
          return Code;
        RefObjQIPtr<IEnum> ClassIdsEnum(ClassIdsEnumProp);
        if (!ClassIdsEnum.Get())
          return Status::Fail;
        RefObjPtr<IBase> ModulePathProp;
        RefObjPtr<IServiceInfo> ServiceInfo;
        if (ServiceItem->GetProperty(ModulePathTag, ModulePathProp.GetPPtr()) == Status::Ok)
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
          RefObjPtr<IBase> RemotePropertiesProp;
          if (ServiceItem->GetProperty(RemotePropertiesTag, RemotePropertiesProp.GetPPtr()) == Status::Ok)
          {
            RefObjQIPtr<IPropertyBag> RemoteProperties(RemotePropertiesProp);
            if (!RemoteProperties.Get())
              return Status::Fail;
            auto RemoteSrvInfo = Base<RemoteServiceInfo>::Create();
            RemoteSrvInfo->SetServiceId(SrvId);
            RemoteSrvInfo->AddCoClassIds(ClassIdsEnum);
            RemoteSrvInfo->SetProps(RemoteProperties);
            ServiceInfo = RemoteSrvInfo;
          }
          else
            return Status::Fail;
        }
        if ((Code = ClassIdsEnum->Reset()) != Status::Ok)
          return Code;
        for (RefObjPtr<IBase> j ; ClassIdsEnum->Next(j.GetPPtr()) == Status::Ok ; j.Release())
        {
          RefObjQIPtr<IEntityId> ClsId(j);
          if (!ClsId.Get())
            return Status::Fail;
          ClassId Id = 0;
          if ((Code = ClsId->GetId(&Id)) != Status::Ok)
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
          if (std::find_if(std::begin(ServiceInfoSet), std::end(ServiceInfoSet),
                [&] (IServiceInfoPtr const &info) { return info.Get() == service.second.Get(); } )
              != std::end(ServiceInfoSet))
          {
            continue;
          }
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
            RefObjPtr<IPropertyBag> RemoteServiceProps;
            if ((Code = RemoteService->GetProperties(RemoteServiceProps.GetPPtr())) != Status::Ok ||
                (Code = ServiceItem->SetProperty(RemotePropertiesTag, RemoteServiceProps.Get())) != Status::Ok)
              return Code;
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
