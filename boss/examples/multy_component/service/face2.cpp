//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     03.05.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#include "face2.h"
#include "core/error_codes.h"

#include <iostream>

namespace MyNs
{
  
  Boss::RetCode BOSS_CALL Face2::Method()
  {
    std::cout << "Face2::Method" << std::endl;
    return Boss::Status::Ok;
  }

}
