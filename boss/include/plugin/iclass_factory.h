//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_PLUGIN_ICLASS_FACTORY_H__
#define __BOSS_PLUGIN_ICLASS_FACTORY_H__

#include "core/ibase.h"

namespace Boss
{
  
  struct IClassFactory
    : public Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID_HEX(0xD14D2E99)  // CRC32(Boss.IClassFactory)
    
    virtual RetCode BOSS_CALL CreateObject(ClassId clsId, IBase **inst) = 0;
  };
  
}

#endif  // !__BOSS_PLUGIN_ICLASS_FACTORY_H__
