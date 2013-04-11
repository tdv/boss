#ifndef __BOSS_COMMON_IISTREAM_H__
#define __BOSS_COMMON_IISTREAM_H__

#include "../core/ibase.h"
#include "ibuffer.h"

namespace Boss
{
  
  struct IIStream
    : Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID("Boss.IIStream")
    virtual RetCode BOSS_CALL Read(UInt bytes, IBuffer **buffer) = 0;
    virtual RetCode BOSS_CALL Read(IBuffer **buffer) = 0;
  };
  
}

#endif  // !__BOSS_COMMON_IISTREAM_H__
