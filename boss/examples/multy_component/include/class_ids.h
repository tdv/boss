//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     03.05.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __MULTY_COMPONENT_CLASS_IDS_H__
#define __MULTY_COMPONENT_CLASS_IDS_H__

#include "core/utils.h"

namespace MyNs
{
  namespace Service
  {
    namespace Id
    {
      enum
      {
        Face1 = 0x1C3FA8AF, // CRC32(MyNs.Service.Id.Face1),
        Face2 = 0x8536F915  // CRC32(MyNs.Service.Id.Face2)
      };
    }
  }
}

#endif  // !__MULTY_COMPONENT_CLASS_IDS_H__
