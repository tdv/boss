#ifndef __BOSS_PLUGIN_ICLASS_FACTORY_H__
#define __BOSS_PLUGIN_ICLASS_FACTORY_H__

#include "core/ibase.h"

namespace Boss
{
  
  struct IClassFactory
    : public Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID("Boss.IClassFactory")
    
    virtual RetCode BOSS_CALL CreateObject(ClassId clsId, IBase **inst) = 0;
  };
  
}

#endif  // !__BOSS_PLUGIN_ICLASS_FACTORY_H__
