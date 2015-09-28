//-------------------------------------------------------------------
//  Base Objects for Service Solutions (BOSS)
//  www.t-boss.ru
//
//  Created:     01.03.2014
//  mail:        boss@t-boss.ru
//
//  Copyright (C) 2014 t-Boss 
//-------------------------------------------------------------------

#ifndef __BOSS_PLUGIN_WIN_DLL_HOLDER_H__
#define __BOSS_PLUGIN_WIN_DLL_HOLDER_H__

#include "core/exceptions.h"
#include "common/utility.h"

#include <string>
#include <utility>

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#include <windows.h>
#include <assert.h>

namespace Boss
{

  BOSS_DECLARE_RUNTIME_EXCEPTION(DllHolder)

  namespace Private
  {

    class DllHolder
    {
    public:
      DllHolder(DllHolder const &) = delete;
      DllHolder& operator = (DllHolder const &) = delete;
      
      DllHolder(const std::string &path)
        : DllInst(LoadLibraryA(path.c_str()))
      {
        if (DllInst == NULL)
          throw DllHolderException("Failed to load \"" + path + "\"");
      }
      DllHolder(DllHolder &&dll)
        : DllInst(dll.DllInst)
      {
        dll.DllInst = 0;
      }
      DllHolder& operator = (DllHolder &&dll)
      {
        UnloadDll();
        DllInst = dll.DllInst;
        dll.DllInst = 0;
        return *this;
      }
      ~DllHolder()
      {
        UnloadDll();
      }

      template <typename T>
      T GetProc(const std::string &procName)
      {
        if (DllInst == NULL)
          throw DllHolderException("Library was not loaded.");
        FARPROC Proc = GetProcAddress(DllInst, procName.c_str());
        if (Proc == NULL)
          throw DllHolderException("Function \"" + procName+ "\" not found.");
        return reinterpret_cast<T>(Proc);
      }

    private:
      HINSTANCE DllInst;
      
      void UnloadDll()
      {
        if (DllInst == NULL)
          return;
        BOOL Res = FreeLibrary(DllInst);
        assert(!!Res && "Failed to unload dll.");
        Unused(Res);
        DllInst = NULL;
      }
    };

  }

  typedef Private::DllHolder DllHolder;

}

#endif  // !__BOSS_PLUGIN_WIN_DLL_HOLDER_H__
