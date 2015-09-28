//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     03.05.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#include "face1.h"
#include "face2.h"

#include "plugin/module.h"

namespace
{
  
  typedef std::tuple
    <
      MyNs::Face1,
      MyNs::Face2
    >
    ExportedCoClasses;

}

BOSS_DECLARE_MODULE_ENTRY_POINT("MultyComponentExample", ExportedCoClasses)
