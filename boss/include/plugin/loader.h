#ifndef __BOSS_PLUGIN_LOADER_H__
#define __BOSS_PLUGIN_LOADER_H__

#include "iservice_registry.h"
#include "iclass_factory.h"
#include "iclass_factory_ctrl.h"
#include "module_holder.h"
#include "service_ids.h"
#include "core/exceptions.h"
#include "common/file_stream.h"
#include "common/iserializable.h"

#include <string>

namespace Boss
{

  BOSS_DECLARE_RUNTIME_EXCEPTION(Loader)
  
  class Loader final
  {
  public:
    Loader(Loader const &) = delete;
    Loader& operator = (Loader const &) = delete;
    
    Loader(std::string const &registryFilePath,
           std::string const &srvRegModulePath,
           std::string const &clsFactoryModulePath)
      : SrvRegistry([&] ()
          {
            auto SrvRegModule(ModuleHolder(std::move(DllHolder(srvRegModulePath))));
            auto SrvReg = SrvRegModule.CreateObject<IServiceRegistry>(Service::Id::ServiceRegistry);
            RefObjQIPtr<ISerializable> Serializable(SrvReg);
            if (!Serializable.Get())
              throw LoaderException("Failed to get ISerializable interface from Registry object.");
            if (Serializable->Load(Base<IFileStream>::Create(registryFilePath).Get()) != Status::Ok)
              throw LoaderException("Failed to load Registry.");
            return std::move(std::make_pair(std::move(SrvRegModule), std::move(SrvReg)));
          } ())
      , ClsFactory([&] ()
          {
            auto ClassFactoryModule(ModuleHolder(std::move(DllHolder(clsFactoryModulePath))));
            auto NewClsFactory = ClassFactoryModule.CreateObject<IClassFactory>(Service::Id::ClassFactory);
            RefObjQIPtr<IClassFactoryCtrl> Ctrl(NewClsFactory);
            if (!Ctrl.Get())
              throw LoaderException("Failed to get ICalssFactoryCtrl interface from ClassFactory object.");
            if (Ctrl->SetRegistry(SrvRegistry.second.Get()) != Status::Ok)
              throw LoaderException("Failed to set Registry into ClassFactory.");
            return std::move(std::make_pair(std::move(ClassFactoryModule), std::move(NewClsFactory)));
          } ())
    {
    }
    template <typename T>
    RefObjPtr<T> CreateObject(ClassId clsId)
    {
      RefObjPtr<IBase> NewInst;
      if (ClsFactory.second->CreateObject(clsId, NewInst.GetPPtr()) != Status::Ok)
        throw LoaderException("Failed to create object.");
      RefObjQIPtr<T> Ret(NewInst);
      if (!Ret.Get())
        throw LoaderException("Interface not found.");
      return Ret;
    }
    ~Loader()
    {
      ClsFactory.second.Release();
      SrvRegistry.second.Release();
    }
    
  private:
    std::pair<ModuleHolder, RefObjPtr<IServiceRegistry>> SrvRegistry;
    std::pair<ModuleHolder, RefObjPtr<IClassFactory>> ClsFactory;
  };
  
}

#endif  // !__BOSS_PLUGIN_LOADER_H__
