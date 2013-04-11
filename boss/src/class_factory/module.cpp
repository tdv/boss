#include "class_factory.h"

#include "plugin/module.h"

namespace
{
  
  typedef std::tuple
    <
      Boss::ClassFactory
    >
    ExportedCoClasses;

}

BOSS_DECLARE_MODULE_ENTRY_POINT("ClassFactory", ExportedCoClasses)
