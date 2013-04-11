#ifndef __BOSS_CORE_BOSS_CALL_H__
#define __BOSS_CORE_BOSS_CALL_H__

#ifdef __GNUC__
  #define BOSS_CALL __attribute__((stdcall)) 
#else
  #error BOSS_CALL not implemented
#endif

#endif  // !__BOSS_CORE_BOSS_CALL_H__
