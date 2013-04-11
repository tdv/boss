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
    BOSS_DECLARE_IFACEID("Boss.ISerializable")
    virtual RetCode BOSS_CALL Load(IIStream *stream) = 0;
    virtual RetCode BOSS_CALL Save(IOStream *stream) = 0;
  };
  
}

#endif  // !__BOSS_COMMON_ISERIALIZABLE_H__
