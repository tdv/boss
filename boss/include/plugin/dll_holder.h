//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_PLUGIN_DLL_HOLDER_H__
#define __BOSS_PLUGIN_DLL_HOLDER_H__

#ifdef _WIN32
#  include "win/dll_holder.h"
#else
#  if (defined(__linux__) || defined (__unix__))
#    include "nix/dll_holder.h"
#  else
#    error "Unknown OS"
#  endif
#endif

#endif  // !__BOSS_PLUGIN_DLL_HOLDER_H__
