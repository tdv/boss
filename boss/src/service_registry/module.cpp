#include "service_registry.h"

#include "plugin/module.h"

namespace
{
  
  typedef std::tuple
    <
      Boss::ServiceRegistry
    >
    ExportedCoClasses;

}

BOSS_DECLARE_MODULE_ENTRY_POINT("ServiceRegistry", ExportedCoClasses)
