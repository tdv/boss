#ifndef __BOSS_CORE_UTILS_H__
#define __BOSS_CORE_UTILS_H__

#include "core_types.h"

namespace Boss
{

  template <typename T>
  struct IntefaceTraits
  {
    typedef T Interface;
    static InterfaceId const Id = Interface::GetInterfaceId();
  };

}

#endif  // !__BOSS_CORE_UTILS_H__
