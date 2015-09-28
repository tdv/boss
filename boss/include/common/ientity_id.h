//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_COMMON_IENTITY_ID_H__
#define __BOSS_COMMON_IENTITY_ID_H__

#include "../core/ibase.h"

namespace Boss
{
  
  struct IEntityId
    : public Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID_HEX(0x252230FA) // CRC32(Boss.IEntityId)
    
    virtual RetCode BOSS_CALL GetId(UInt *id) const = 0;
  };
  
}

# endif  // !__BOSS_COMMON_IENTITY_ID_H__
