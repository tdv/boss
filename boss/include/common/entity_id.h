#ifndef __BOSS_COMMON_ENTITYID_H__
#define __BOSS_COMMON_ENTITYID_H__

#include "../core/co_class.h"
#include "../core/error_codes.h"
#include "ientity_id.h"

namespace Boss
{
  
  class EntityId
    : public CoClass<Crc32("Boss.EntityId"), IEntityId>
  {
  public:
    EntityId(UInt id = 0)
      : Id(id)
    {
    }
    void SetId(UInt id)
    {
      Id = id;
    }
    
  private:
    UInt Id;
    
    // IEntityId
    virtual RetCode BOSS_CALL GetId(UInt *id) const
    {
      if (!id)
        return Status::InvalidArgument;
      *id = Id;
      return Status::Ok;
    }
  };
  
}

#endif  // !__BOSS_COMMON_ENTITYID_H__
