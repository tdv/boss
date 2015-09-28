//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#include "calc_service.h"
#include "core/error_codes.h"

namespace MyNs
{
  
  Boss::RetCode BOSS_CALL Sum::CalcSum(int a, int b, int *sum)
  {
    if (*sum)
      return Boss::Status::InvalidArgument;
    *sum = a + b;
    return Boss::Status::Ok;
  }

}
