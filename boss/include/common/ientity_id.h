#ifndef __BOSS_COMMON_IENTITY_ID_H__
#define __BOSS_COMMON_IENTITY_ID_H__

#include "../core/ibase.h"

namespace Boss
{
  
  struct IEntityId
    : public Inherit<IBase>
  {
    BOSS_DECLARE_IFACEID("Boss.IEntityId")
    
    virtual RetCode BOSS_CALL GetId(UInt *id) const = 0;
  };
  
}

#endif  // !__BOSS_COMMON_IENTITY_ID_H__
