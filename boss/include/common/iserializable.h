//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_COMMON_ISERIALIZABLE_H__
#define __BOSS_COMMON_ISERIALIZABLE_H__

#include "iistream.h"
#include "iostream.h"
#include "../core/ibase.h"

namespace Boss
{
  
  struct ISerializable
    : public Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID_HEX(0x28C07F90)  // CRC32(Boss.ISerializable)
    
    virtual RetCode BOSS_CALL Load(IIStream *stream) = 0;
    virtual RetCode BOSS_CALL Save(IOStream *stream) = 0;
  };
  
}

#endif  // !__BOSS_COMMON_ISERIALIZABLE_H__
