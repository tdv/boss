#ifndef __BOSS_PLUGIN_DLL_HOLDER_H__
#define __BOSS_PLUGIN_DLL_HOLDER_H__

#ifdef _WIN32
#  include "win/dll_holder.h"
#else
#  ifdef __linux__
#    include "nix/dll_holder.h"
#  else
#    error "Unknown OS"
#  endif
#endif

#endif  // !__BOSS_PLUGIN_DLL_HOLDER_H__
