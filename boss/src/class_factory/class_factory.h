//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_PLUGIN_CLASS_FACTORY_H__
#define __BOSS_PLUGIN_CLASS_FACTORY_H__

#include "core/co_class.h"
#include "core/ref_obj_ptr.h"
#include "core/exceptions.h"
#include "common/iservice_locator.h"
#include "plugin/iclass_factory.h"
#include "plugin/iclass_factory_ctrl.h"
#include "plugin/service_ids.h"

#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>

namespace Boss
{

  BOSS_DECLARE_RUNTIME_EXCEPTION(ClassFactory)
  
  class ClassFactory
    : public CoClass
        <
          Service::Id::ClassFactory,
          IClassFactory,
          IClassFactoryCtrl
        >
  {
  public:
    ClassFactory();
    virtual ~ClassFactory();

    void FinalizeConstruct();
    void BeforeRelease();
    
    // IClassFactory
    virtual RetCode BOSS_CALL CreateObject(ClassId clsId, IBase **inst);

    // IClassFactoryCtrl
    virtual RetCode BOSS_CALL SetRegistry(IServiceRegistry *registry);    
    
  private:
    std::recursive_mutex Mtx;

    class ModuleHolderEx;
    typedef std::shared_ptr<ModuleHolderEx> ModulePtr;
    typedef std::unordered_map<ClassId, ServiceId> ServicePool;
    typedef std::unordered_map<ServiceId, ModulePtr> ModulePool;
    
    ServicePool Services;
    ModulePool Modules;
    
    bool volatile IsRunCleaner = true;
    unsigned const CleanerTimeout = 1;
    std::unique_ptr<std::thread> CleanerThread;
    
    RefObjPtr<IServiceRegistry> Registry;
    RefObjPtr<IServiceLocator> Locator;
    
    void Clean();
  };
  
}

#endif  // !__BOSS_PLUGIN_CLASS_FACTORY_H__
