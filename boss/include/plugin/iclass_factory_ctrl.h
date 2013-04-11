#ifndef __BOSS_PLUGIN_ICLASS_FACTORY_CTRL_H__
#define __BOSS_PLUGIN_ICLASS_FACTORY_CTRL_H__

#include "core/ibase.h"
#include "iservice_registry.h"

namespace Boss
{
  
  struct IClassFactoryCtrl
    : public Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID("Boss.IClassFactoryCtrl")
    
    virtual RetCode BOSS_CALL SetRegistry(IServiceRegistry *registry) = 0;
  };
  
}

#endif  // !__BOSS_PLUGIN_ICLASS_FACTORY_CTRL_H__
