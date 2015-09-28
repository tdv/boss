//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_CORE_ERROR_CODES_H__
#define __BOSS_CORE_ERROR_CODES_H__

namespace Boss
{

  namespace Status
  {

    enum
    {
      Ok,
      False,
      Fail,
      NotFound,
      AlreadyExists,
      InvalidArgument,
      NotImplemented,
      NotInitialized,
      MethodNotFound
    };

  }

}

#endif  // !__BOSS_CORE_ERROR_CODES_H__
