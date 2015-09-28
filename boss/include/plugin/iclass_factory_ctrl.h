//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_PLUGIN_ICLASS_FACTORY_CTRL_H__
#define __BOSS_PLUGIN_ICLASS_FACTORY_CTRL_H__

#include "core/ibase.h"
#include "iservice_registry.h"

namespace Boss
{
  
  struct IClassFactoryCtrl
    : public Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID_HEX(0xE488C88A)  // CRC32(Boss.IClassFactoryCtrl)
    
    virtual RetCode BOSS_CALL SetRegistry(IServiceRegistry *registry) = 0;
  };
  
}

#endif  // !__BOSS_PLUGIN_ICLASS_FACTORY_CTRL_H__
