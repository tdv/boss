//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_PLUGIN_SERVICE_IDS_H__
#define __BOSS_PLUGIN_SERVICE_IDS_H__

#include "../core/utils.h"

namespace Boss
{
  
  namespace Service
  {
    
    namespace Id
    {
      
      enum
      {
        ServiceRegistry = 0x2B89E12A, // CRC32(Boss.ServiceRegistry)
        ClassFactory = 0x5E8910F4 // CRC32(Boss.ClassFactory)
      };
      
    }
    
  }
  
}

#endif  // !__BOSS_PLUGIN_SERVICE_IDS_H__
