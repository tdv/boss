//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#include "class_factory.h"
#include "core/error_codes.h"
#include "common/string_helper.h"
#include "plugin/module_holder.h"
#include "plugin/factory_tools.h"
#include "plugin/service_locator_ids.h"

#include <chrono>
#include <set>

#include <assert.h>

namespace Boss
{
  
  class ClassFactory::ModuleHolderEx final
  {
  public:
    ModuleHolderEx(ModuleHolderEx const &) = delete;
    ModuleHolderEx& operator = (ModuleHolderEx const &) = delete;
    
    ModuleHolderEx(std::string const &path)
      : Module(std::move(DllHolder(path)))
    {
      auto Locator = Private::GetServiceLocator();
      if (Locator.Get())
      {
        Module.SetServiceLocator(Locator.Get());
        LocatorExists = false;
      }
    }
    ~ModuleHolderEx()
    {
      if (LocatorExists)
      {
        try
        {
          Module.SetServiceLocator(0);
        }
        catch (std::exception const &)
        {
        }
      }
    }
    ModuleHolder& GetModule()
    {
      return Module;
    }
    
  private:
    bool LocatorExists = false;
    ModuleHolder Module;
  };

  ClassFactory::ClassFactory()
  {
  }
  
  ClassFactory::~ClassFactory()
  {
  }

  void ClassFactory::FinalizeConstruct()
  {
    Locator = Boss::Private::GetServiceLocator();
    if (Locator.Get() && Locator->AddService(Service::Locator::Id::ClassFactoryService, GetThisIBase()))
      throw ClassFactoryException("Failed to put ClassFactory into ServiceLocator");
    
    //CleanerThread.reset(new std::thread(&ClassFactory::Clean, this));
  }
  
  void ClassFactory::BeforeRelease()
  {
    std::lock_guard<std::recursive_mutex> Lock(Mtx);
    IsRunCleaner = false;
    //CleanerThread->join();
    CleanerThread.reset();
    Registry.Release();
    for (auto &i : Modules)
    {
      try
      {
        assert(!i.second->GetModule().GetRefCount() && "Module has not null reference counter.");
      }
      catch (...)
      {
        assert(false && "Failed to get module reference counter.");
      }
    }
    if (Locator.Get())
      Locator->DelService(Service::Locator::Id::ClassFactoryService);
  }  

  RetCode BOSS_CALL ClassFactory::CreateObject(ClassId clsId, IBase **inst)
  {
    try
    {
      std::lock_guard<std::recursive_mutex> Lock(Mtx);
      {
        auto const ServiceIter = Services.find(clsId);
        if (ServiceIter != std::end(Services))
        {
          auto ModuleIter = Modules.find(ServiceIter->second);
          if (ModuleIter == std::end(Modules))
            return Status::Fail;
          return ModuleIter->second->GetModule().CreateObject(clsId).QueryInterface(inst);
        }
      }
      if (!Registry.Get())
        return Status::Fail;
      RefObjPtr<IServiceInfo> SrvInfo;
      RetCode Code = Registry->GetServiceInfo(clsId, SrvInfo.GetPPtr());
      if (Code != Status::Ok)
        return Code;
      {
        RefObjQIPtr<ILocalServiceInfo> LocalSrvInfo(SrvInfo);
        if (LocalSrvInfo.Get())
        {
          std::string Path;
          {
            RefObjPtr<IString> ModulePath;
            if ((Code = LocalSrvInfo->GetModulePath(ModulePath.GetPPtr())) != Status::Ok)
              return Code;
            Path = std::move(StringHelper(ModulePath).GetString<IString::AnsiString>());
          }
          auto Module = std::make_shared<ModuleHolderEx>(Path);
          auto NewInst = Module->GetModule().CreateObject(clsId);
          auto SrvId = Module->GetModule().GetServiceId();
          for (auto const &i : Module->GetModule().GetClassIds())
            Services[i] = SrvId;
          Modules.insert(std::make_pair(SrvId, std::move(Module)));
          return NewInst.QueryInterface(inst);
        }
      }
      {
        RefObjQIPtr<IRemoteServiceInfo> RemoteSrvInfo(SrvInfo);
        if (RemoteSrvInfo.Get())
        {
          return Status::NotImplemented;
        }
      }
    }
    catch (...)
    {
      return Status::Fail;
    }
    return Status::NotFound;
  }
  
  RetCode BOSS_CALL ClassFactory::SetRegistry(IServiceRegistry *registry)
  {
    try
    {
      std::lock_guard<std::recursive_mutex> Lock(Mtx);
      Registry = registry;
    }
    catch (...)
    {
      return Status::Fail;
    }
    return Status::Ok;
  }
  
  void ClassFactory::Clean()
  {
    while (IsRunCleaner)
    {
      try
      {
        std::this_thread::sleep_for(std::chrono::seconds(CleanerTimeout));
        if (!IsRunCleaner)
          break;
        
        std::lock_guard<std::recursive_mutex> Lock(Mtx);
        std::set<ServiceId> ServicesForUnload;
        for (auto const & i : Modules)
        {
          try
          {
            if (!i.second->GetModule().GetRefCount())
              ServicesForUnload.insert(i.first);
          }
          catch (std::exception const &)
          {
            // TODO :
          }
        }
        std::vector<ClassId> ClassIdsForDelete;
        for (auto const &i : Services)
        {
          if (ServicesForUnload.find(i.second) != std::end(ServicesForUnload))
            ClassIdsForDelete.push_back(i.first);
        }
        for (auto const &i : ServicesForUnload)
          Modules.erase(i);
        for (auto const &i : ClassIdsForDelete)
          Services.erase(i);
      }
      catch (...)
      {
      }
    }
  }

}
