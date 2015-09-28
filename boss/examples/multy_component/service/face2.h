//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     03.05.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __MULTY_COMPONENT_FACE2_H__
#define __MULTY_COMPONENT_FACE2_H__

#include "ifaces.h"
#include "class_ids.h"
#include "core/co_class.h"

namespace MyNs
{

  class Face2
    : public Boss::CoClass
          <
            Service::Id::Face2,
            IFace2
          >
  {
  public:
    // IFace2
    virtual Boss::RetCode BOSS_CALL Method();
  };
  
}

#endif  // !__MULTY_COMPONENT_FACE2_H__
