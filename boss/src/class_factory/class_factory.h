#ifndef __BOSS_PLUGIN_CLASS_FACTORY_H__
#define __BOSS_PLUGIN_CLASS_FACTORY_H__

#include "core/co_class.h"
#include "core/ref_obj_ptr.h"
#include "plugin/iclass_factory.h"
#include "plugin/iclass_factory_ctrl.h"
#include "plugin/service_ids.h"
#include "plugin/module_holder.h"

#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>

namespace Boss
{
  
  class ClassFactory
    : public CoClass<Service::Id::ClassFactory, IClassFactory, IClassFactoryCtrl>
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

    typedef std::shared_ptr<ModuleHolder> ModulePtr;
    typedef std::unordered_map<ClassId, ServiceId> ServicePool;
    typedef std::unordered_map<ServiceId, ModulePtr> ModulePool;
    ServicePool Services;
    ModulePool Modules;
    
    bool volatile IsRunCleaner = true;
    unsigned const CleanerTimeout = 1;
    std::unique_ptr<std::thread> CleanerThread;
    
    RefObjPtr<IServiceRegistry> Registry;
    
    void Clean();
  };
  
}

#endif  // !__BOSS_PLUGIN_CLASS_FACTORY_H__
