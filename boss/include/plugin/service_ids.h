#ifndef __BOSS_PLUGIN_SERVICE_IDS_H__
#define __BOSS_PLUGIN_SERVICE_IDS_H__

#include "../core/crc32.h"

namespace Boss
{
  
  namespace Service
  {
    
    namespace Id
    {
      
      enum
      {
        ServiceRegistry = Crc32("Boss.ServiceRegistry"),
        ClassFactory = Crc32("Boss.ClassFactory")
      };
      
    }
    
  }
  
}

#endif  // !__BOSS_PLUGIN_SERVICE_IDS_H__
