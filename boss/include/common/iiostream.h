#ifndef __BOSS_COMMON_IIOSTREAM_H__
#define __BOSS_COMMON_IIOSTREAM_H__

#include "iistream.h"
#include "iostream.h"

namespace Boss
{
  
  struct IIOStream
    : Inherit<IIStream, IOStream>
  {
    BOSS_DECLARE_IFACEID("Boss.IIOStream")
  };
  
}

#endif  // !__BOSS_COMMON_IIOSTREAM_H__
