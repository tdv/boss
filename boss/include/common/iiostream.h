//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_COMMON_IIOSTREAM_H__
#define __BOSS_COMMON_IIOSTREAM_H__

#include "iistream.h"
#include "iostream.h"

namespace Boss
{
  
  struct IIOStream
    : Inherit<IIStream, IOStream>
  {
    BOSS_DECLARE_IFACEID_HEX(0x8D4862E6)  // CRC32(Boss.IIOStream)
  };
  
}

#endif  // !__BOSS_COMMON_IIOSTREAM_H__
