#include "class_factory.h"
#include "core/error_codes.h"

#include <chrono>
#include <set>

#include <assert.h>

namespace Boss
{

  ClassFactory::ClassFactory()
  {
  }
  
  ClassFactory::~ClassFactory()
  {
  }

  void ClassFactory::FinalizeConstruct()
  {
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
        assert(!i.second->GetRefCount() && "Module has not null reference counter.");
      }
      catch (...)
      {
        assert(false && "Failed to get module reference counter.");
      }
    }
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
          return ModuleIter->second->CreateObject(clsId).QueryInterface(inst);
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
            RefObjPtr<IBuffer> StrBuffer;
            if ((Code = ModulePath->GetString(IString::AnsiString, StrBuffer.GetPPtr())) != Status::Ok)
              return Code;
            UInt StrLen = 0;
            void const *StrData = 0;
            if ((Code = StrBuffer->GetSize(&StrLen)) != Status::Ok ||
                (Code = StrBuffer->GetData(&StrData)) != Status::Ok)
            {
              return Code;
            }
            Path.assign(reinterpret_cast<char const *>(StrData), StrLen);
          }
          auto Module = std::make_shared<ModuleHolder>(std::move(DllHolder(Path)));
          auto NewInst = Module->CreateObject(clsId);
          auto SrvId = Module->GetServiceId();
          for (auto const &i : Module->GetClassIds())
            Services[i] = SrvId;
          Modules.insert(std::make_pair(SrvId, std::move(Module)));
          return NewInst.QueryInterface(inst);
        }
      }
      {
        RefObjQIPtr<IRemoteServiceInfo> RemoteSrvInfo(SrvInfo);
        if (RemoteSrvInfo.Get())
        {
          // TODO::
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
            if (!i.second->GetRefCount())
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
