//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_CORE_BOSS_CALL_H__
#define __BOSS_CORE_BOSS_CALL_H__

#ifdef __GNUC__
  #ifdef __x86_64__
    #define BOSS_CALL 
  #else
    #define BOSS_CALL __attribute__((stdcall)) 
  #endif
#elif _MSC_VER
  #define BOSS_CALL __stdcall 
#else
  #error BOSS_CALL not implemented
#endif

#endif  // !__BOSS_CORE_BOSS_CALL_H__
