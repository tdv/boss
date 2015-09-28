//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

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
